#include "../WICTextureLoader.h"
#include "FbxMeshFile.h"
#include <DirectXTex.h>
#include <codecvt> 

//コンストラクタ
FbxMeshFile::FbxMeshFile():
	m_InputLayout(nullptr)
{
	m_MeshList.clear();
}

//デストラクタ
FbxMeshFile::~FbxMeshFile()
{
	for (auto texture : m_Textures)
	{
		if (texture.second != nullptr)
		{
			texture.second->Release();
		}
	}
	m_Textures.clear();
	m_MaterialLinks.clear();

	for (auto mesh : m_MeshList)
	{
		if (mesh.m_VertexBuffer != nullptr)
		{
			mesh.m_VertexBuffer->Release();
		}

		if (mesh.m_IndexBuffer != nullptr)
		{
			mesh.m_IndexBuffer->Release();
		}
		mesh.m_Indices.clear();
		mesh.m_Vertices.clear();
	}

	if (m_InputLayout != nullptr)
	{
		m_InputLayout->Release();
		m_InputLayout = nullptr;
	}
}

//モデル読み込み関数
bool FbxMeshFile::Load(const char* file_name, ID3D11Device* device, VertexShader* vertex_shader)
{
	////メッシュ生成関数
	if (LoadFbxFail(file_name) == false)
	{
		return false;
	}

	//VertexBuffer作成関数
	if (CreateVertexBuffer(device) == false)
	{
		return false;
	}

	//IndexBuffer作成関数
	if (CreateIndexBuffer(device) == false)
	{
		return false;
	}

	//入力レイアウト作成関数
	if (CreateInputLayout(device, vertex_shader) == false)
	{
		return false;
	}

	return true;
}

//メッシュ生成関数
bool FbxMeshFile::LoadFbxFail(const char* file_name)
{
	// FbxManager作成
	FbxManager* fbx_manager = fbxsdk::FbxManager::Create();
	if (fbx_manager == nullptr)
	{
		return false;
	}

	// FbxImporter作成
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	if (fbx_importer == nullptr)
	{
		fbx_manager->Destroy();
		return false;
	}

	// FbxScene作成
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "");
	if (fbx_scene == nullptr)
	{
		fbx_importer->Destroy();
		fbx_manager->Destroy();
		return false;
	}

	// Fileを初期化
	fbx_importer->Initialize(file_name);
	// sceneにインポート
	fbx_importer->Import(fbx_scene);

	//fbx_importer->Destroy();

	FbxGeometryConverter converter(fbx_manager);
	// メッシュに使われているマテリアル単位でメッシュを分割する
	converter.SplitMeshesPerMaterial(fbx_scene, true);
	// ポリゴンを三角形にする
	converter.Triangulate(fbx_scene, true);

	int material_num = fbx_scene->GetSrcObjectCount<FbxSurfaceMaterial>();

	for (int i = 0; i < material_num; i++)
	{
		//マテリアル読み込み
		LoadMaterial(fbx_scene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	std::map<int, std::string> non_skin_list;

#pragma region ノード探査法
	/*FbxNode* rootNode = fbx_scene->GetRootNode();

	if (rootNode != nullptr)
	{
		for (int i = 0; rootNode->GetChildCount() > i; i++)
		{
			const char* name = rootNode->GetName();
			ProbeNode(rootNode->GetChild(i), non_skin_list);
		}
	}*/
#pragma endregion

	// FbxMeshの数を取得
	int mesh_num = fbx_scene->GetSrcObjectCount<FbxMesh>();

	for (int i = 0; i < mesh_num; i++)
	{
		// Mesh作成
		CreateMesh(fbx_scene->GetSrcObject<FbxMesh>(i), non_skin_list);
	}

	for (auto non_skin : non_skin_list)
	{
		m_MeshList[non_skin.first].m_ParentBoneId = FindBone(non_skin.second.c_str());
	}

	return true;
}

//メッシュデータ検索関数
bool FbxMeshFile::IsMesh(FbxNode* node_)
{
	if (node_ == nullptr)
	{
		return false;
	}

	//Attribute数取得
	int attrCount = node_->GetNodeAttributeCount();

	for (int i = 0; attrCount > i; i++)
	{
		FbxNodeAttribute::EType attrType = node_->GetNodeAttributeByIndex(i)->GetAttributeType();

		if (attrType == FbxNodeAttribute::EType::eMesh)
		{
			return true;
		}
	}

	return false;
}

//ノード探索関数
void FbxMeshFile::ProbeNode(FbxNode* node_, std::map<int, std::string>& non_skin_list)
{
	if (node_ != nullptr)
	{
		//メッシュデータ検索
		if (IsMesh(node_) == true)
		{
			FbxMesh* mesh = node_->GetMesh();

			if (mesh != nullptr)
			{
				//メッシュデータ作成
				CreateMesh(mesh, non_skin_list);
			}
		}
	}

	//子の分だけ回す
	for (int i = 0; node_->GetChildCount() > i; i++)
	{
		ProbeNode(node_->GetChild(i), non_skin_list);
	}

}

//モデル描画関数
void FbxMeshFile::Render(DirectX::XMMATRIX world_matrix_, DrawType draw_type_)
{
	UINT strides = sizeof(CustomVertex);
	UINT offsets = 0;

	// ボーン行列
	Motion* motion = &m_Motion[m_PlayMotion];
	if (motion != nullptr)
	{
		if (motion->FrameNum >= 0)
		{
			float frame = m_Frame;
			int f = static_cast<int>(frame);

			for (UINT b = 0; b < m_BoneNum; ++b)
			{
				if (motion->Key[b].empty()) {
					DirectGraphics::Instance()->GetConstantBufferData()->Bone[b] = DirectX::XMMatrixIdentity();
					continue;
				}

				m_Bone[b].Transform = motion->Key[b][f] * (1.0f - (frame - static_cast<int>(frame))) + motion->Key[b][f + 1] * (frame - static_cast<int>(frame));
				DirectX::XMMATRIX mat = m_Bone[b].Offset * m_Bone[b].Transform;
				DirectGraphics::Instance()->GetConstantBufferData()->Bone[b] = DirectX::XMMatrixTranspose(mat);
			}
		}
	}

	for (auto& mesh : m_MeshList)
	{
		// インデックスバッファの数 = マテリアルの数だけメッシュを描画する
		// IA(InputAssemblerStage)に入力レイアウトを設定する
		DirectGraphics::Instance()->GetContext()->IASetInputLayout(m_InputLayout);
		// IAに設定する頂点バッファの指定
		DirectGraphics::Instance()->GetContext()->IASetVertexBuffers(
			0,								// バッファ送信のスロット番号
			1,								// バッファの数
			&mesh.m_VertexBuffer,	// 頂点バッファ
			&strides,						// バッファに使用している構造体のサイズ
			&offsets);						// 開始オフセット

		DirectGraphics::Instance()->GetContext()->IASetIndexBuffer(
			mesh.m_IndexBuffer,
			DXGI_FORMAT_R32_UINT,
			0);

		//ボーンに親子関係がない場合
		if (mesh.m_ParentBoneId < 0)
		{
			// ワールドマトリクスをコンスタントバッファに設定
			XMStoreFloat4x4(&DirectGraphics::Instance()->GetConstantBufferData()->World, XMMatrixTranspose(world_matrix_));
		}
		else
		{
			DirectX::XMMATRIX new_mat = m_Bone[mesh.m_ParentBoneId].Offset * m_Bone[mesh.m_ParentBoneId].Transform * world_matrix_;
			XMStoreFloat4x4(&DirectGraphics::Instance()->GetConstantBufferData()->World, XMMatrixTranspose(new_mat));
		}

		//ボーンに親子関係がない場合
		if (mesh.m_ParentBoneId < 0)
		{
			//モーションがある場合
			if (motion != nullptr && motion->FrameNum > 0)
			{
				//描画タイプによってシェーダーを切り替える
				switch (draw_type_)
				{
				case DrawType::MainDraw:  //メイン
					ShaderManager::Instance()->SetUpShader(VertexShaderType::SkinVertex, PixelShaderType::NomalPixel);
					break;
				case DrawType::ShadowDraw: //影
					ShaderManager::Instance()->SetUpShader(VertexShaderType::ShadowSkinVertex, PixelShaderType::ShadowTexPixel);
					break;
				default:
					break;
				}
			}
			else
			{
				//描画タイプによってシェーダーを切り替える
				switch (draw_type_)
				{
				case DrawType::MainDraw:  //メイン
					ShaderManager::Instance()->SetUpShader(VertexShaderType::StaticVertex, PixelShaderType::NomalPixel);
					break;
				case DrawType::ShadowDraw: //影
					ShaderManager::Instance()->SetUpShader(VertexShaderType::ShadowStaticVertex, PixelShaderType::ShadowTexPixel);
					break;
				default:
					break;
				}
			}
		}
		//ボーンがないオブジェクト(Block等)
		else
		{
			//描画タイプによってシェーダーを切り替える
			switch (draw_type_)
			{
			case DrawType::MainDraw:  //メイン
				ShaderManager::Instance()->SetUpShader(VertexShaderType::StaticVertex, PixelShaderType::NomalPixel);
				break;
			case DrawType::ShadowDraw: //影
				ShaderManager::Instance()->SetUpShader(VertexShaderType::ShadowStaticVertex, PixelShaderType::ShadowTexPixel);
				break;
			default:
				break;
			}
		}

		//マテリアルデータ保存
		SetMaterialColor(DirectGraphics::Instance(), m_Materials[mesh.m_MaterialName]);

		// コンスタントバッファ更新
		DirectGraphics::Instance()->GetContext()->UpdateSubresource(DirectGraphics::Instance()->GetConstantBuffer(), 0, nullptr, DirectGraphics::Instance()->GetConstantBufferData(), 0, 0);

		ID3D11Buffer* constant_buffer = DirectGraphics::Instance()->GetConstantBuffer();
		// コンテキストにコンスタントバッファを設定
		DirectGraphics::Instance()->GetContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
		DirectGraphics::Instance()->GetContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

		// テクスチャ設定
		if (m_MaterialLinks.count(mesh.m_MaterialName) > 0)
		{
			DirectGraphics::Instance()->SetTexture(m_MaterialLinks[mesh.m_MaterialName]);
		}
		else
		{
			DirectGraphics::Instance()->SetTexture(nullptr);
		}

		// 描画
		DirectGraphics::Instance()->GetContext()->DrawIndexed(
			(UINT)mesh.m_Indices.size(),	// 頂点数
			0,								// オフセット
			0);								// 開始頂点のインデックス
	}
}

//メッシュデータ作成関数
void FbxMeshFile::CreateMesh(FbxMesh* mesh, std::map<int, std::string>& non_skin_list)
{
	MeshData mesh_data;
	mesh_data.m_ParentBoneId = -1;

	//頂点インデックスデータ読み込み関数
	LoadIndices(mesh_data, mesh);
	//頂点データ読み込み関数
	LoadVertices(mesh_data, mesh);
	//法線データ読み込み関数
	LoadNormals(mesh_data, mesh);
	//UVデータ読み込み関数
	LoadUV(mesh_data, mesh);
	//頂点カラーデータ読み込み関数
	LoadColors(mesh_data, mesh);
	//マテリアル名読み込み関数
	SetMaterialName(mesh_data, mesh);
	//ボーンデータ読み込み関数
	LoadBones(mesh_data, mesh);

	//メッシュデータ追加
	m_MeshList.push_back(mesh_data);

	int skin_num = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skin_num <= 0)
	{
		FbxNode* parent = mesh->GetNode()->GetParent();
		
		if (parent != nullptr)
		{
			const char* initial_name = parent->GetInitialName();

			non_skin_list[(int)m_MeshList.size() - 1] = initial_name;
		}
	}
}

//頂点データ読み込み関数
void FbxMeshFile::LoadVertices(MeshData& mesh_data, FbxMesh* mesh)
{
	// 頂点バッファの取得
	FbxVector4* vertices = mesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = mesh->GetPolygonVertices();
	// 頂点座標の数の取得
	int polygon_vertex_count = mesh->GetPolygonVertexCount();
	// GetPolygonVertexCount => 頂点数
	for (int i = 0; i < polygon_vertex_count; i++)
	{
		CustomVertex vertex;
		// インデックスバッファから頂点番号を取得
		int index = indices[i];

		// 頂点座標リストから座標を取得する
		vertex.Position.x = (float)-vertices[index][0];
		vertex.Position.y = (float)vertices[index][1];
		vertex.Position.z = (float)vertices[index][2];

		// 追加
		mesh_data.m_Vertices.push_back(vertex);
	}
}

//頂点インデックスデータ読み込み関数
void FbxMeshFile::LoadIndices(MeshData& mesh_data, FbxMesh* mesh)
{
	// ポリゴン数の取得
	int polygon_count = mesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (int i = 0; i < polygon_count; i++)
	{
		mesh_data.m_Indices.push_back(i * 3 + 2);
		mesh_data.m_Indices.push_back(i * 3 + 1);
		mesh_data.m_Indices.push_back(i * 3);
	}
}

//法線データ読み込み関数
void FbxMeshFile::LoadNormals(MeshData& mesh_data, FbxMesh* mesh)
{
	FbxArray<FbxVector4> normals;
	// 法線リストの取得
	mesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (int i = 0; i < normals.Size(); i++)
	{
		mesh_data.m_Vertices[i].Normal.x = (float)-normals[i][0];
		mesh_data.m_Vertices[i].Normal.y = (float)normals[i][1];
		mesh_data.m_Vertices[i].Normal.z = (float)normals[i][2];
	}
}

//頂点カラーデータ読み込み関数
void FbxMeshFile::LoadColors(MeshData& mesh_data, FbxMesh* mesh)
{
	// 頂点カラーデータの数を確認
	int color_count = mesh->GetElementVertexColorCount();
	if (color_count == 0)
	{
		return;
	}

	// 頂点カラーデータの取得
	FbxGeometryElementVertexColor* vertex_colors = mesh->GetElementVertexColor(0);

	if (vertex_colors == nullptr)
	{
		return;
	}

	FbxLayerElement::EMappingMode mapping_mode = vertex_colors->GetMappingMode();
	FbxLayerElement::EReferenceMode reference_mode = vertex_colors->GetReferenceMode();

	if (mapping_mode == FbxLayerElement::eByPolygonVertex)
	{
		if (reference_mode == FbxLayerElement::eIndexToDirect)
		{
			// 頂点カラーバッファ取得
			FbxLayerElementArrayTemplate<FbxColor>& colors = vertex_colors->GetDirectArray();
			// 頂点カラーインデックスバッファ取得
			FbxLayerElementArrayTemplate<int>& indeces = vertex_colors->GetIndexArray();
			for (int i = 0; i < indeces.GetCount(); i++)
			{
				int id = indeces.GetAt(i);
				FbxColor color = colors.GetAt(id);
				mesh_data.m_Vertices[i].Color.Alpha = (float)color.mAlpha;
				mesh_data.m_Vertices[i].Color.Red = (float)color.mRed;
				mesh_data.m_Vertices[i].Color.Green = (float)color.mGreen;
				mesh_data.m_Vertices[i].Color.Blue = (float)color.mBlue;
			}
		}
	}
}

//UVデータ読み込み関数
void FbxMeshFile::LoadUV(MeshData& mesh_data, FbxMesh* mesh)
{
	FbxStringList uvset_names;
	// UVSetの名前リストを取得
	mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSetの名前からUVSetを取得する
	// 今回はマルチテクスチャには対応しないので最初の名前を使う
	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);

	for (int i = 0; i < uv_buffer.Size(); i++)
	{
		FbxVector2& uv = uv_buffer[i];

		// そのまま使用して問題ない
		mesh_data.m_Vertices[i].TexturePos.x = (float)uv[0];
		mesh_data.m_Vertices[i].TexturePos.y = (float)(1.0 - uv[1]);
	}
}

//マテリアル名読み込み関数
void FbxMeshFile::SetMaterialName(MeshData& mesh_data, FbxMesh* mesh)
{
	// マテリアルが無ければ終わり
	if (mesh->GetElementMaterialCount() == 0)
	{
		mesh_data.m_MaterialName = "";
		return;
	}

	// Mesh側のマテリアル情報を取得
	FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);
	int index = material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* surface_material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
	if (surface_material != nullptr)
	{
		mesh_data.m_MaterialName = surface_material->GetName();
	}
	else
	{
		mesh_data.m_MaterialName = "";
	}
}

//テクスチャデータ読み込み関数
bool FbxMeshFile::LoadTexture(FbxFileTexture* texture, std::string& keyword)
{
	if (texture == nullptr)
	{
		return false;
	}

	// ファイル名を取得
	std::string file_path = texture->GetRelativeFileName();

	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_path.c_str(), sizeof(char) * 256);

	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);

	std::string root_path = "Res/FBX/Texture/";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	if (split_list[split_list.size() - 1] == "face_00.tga")
	{
		wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);
	}

	// 文字化け対策
	char* file_name;
	size_t size = 0;
	FbxUTF8ToAnsi(split_list[split_list.size() - 1].c_str(), file_name, &size);

	if (m_Textures.count(file_name) > 0 &&
		m_Textures[file_name] != nullptr)
	{
		keyword = file_name;
		FbxFree(file_name);
		return true;
	}

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;

	if (strstr(file_name, ".tga") != nullptr)
	{
		HRESULT hr = DirectX::LoadFromTGAFile(wstr_file_name.c_str(), &metadata, image);

		if (FAILED(hr))
		{
			FbxFree(file_name);
			return false;
		}

		// 画像からシェーダリソースViewの作成
		hr = CreateShaderResourceView(
			DirectGraphics::Instance()->GetDevice(),
			image.GetImages(),
			image.GetImageCount(),
			metadata,
			&m_Textures[file_name]);

		if (FAILED(hr))
		{
			FbxFree(file_name);
			return false;
		}
	}
	else if (strstr(file_name, ".png") != nullptr)
	{
		if (FAILED(DirectX::CreateWICTextureFromFile(
			DirectGraphics::Instance()->GetDevice(),			// ID3D11Device
			wstr_file_name.c_str(),								// ファイル名(wchar_t型なので注意)
			nullptr,											// 通常のテクスチャデータ
			&m_Textures[file_name])))							// シェーダで使用することができるテクスチャデータ
		{
			FbxFree(file_name);
			return false;
		}
	}


	keyword = file_name;
	FbxFree(file_name);
	return true;
}

//マテリアルデータ読み込み関数
void FbxMeshFile::LoadMaterial(FbxSurfaceMaterial* material)
{
	ObjMaterial entry_material;
	enum class MaterialOrder
	{
		Ambient,
		Diffuse,
		Specular,
		MaxOrder,
	};

	FbxDouble3 colors[(int)MaterialOrder::MaxOrder];
	FbxDouble factors[(int)MaterialOrder::MaxOrder];
	FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
	FbxSurfaceMaterial::sTransparencyFactor;
	FbxSurfaceMaterial::sTransparentColor;
	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		const char* element_check_list[] =
		{
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sDiffuse,
		};

		const char* factor_check_list[] =
		{
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor,
		};

		for (int i = 0; i < 2; i++)
		{
			prop = material->FindProperty(element_check_list[i]);
			if (prop.IsValid())
			{
				colors[i] = prop.Get<FbxDouble3>();

			}
			else
			{
				colors[i] = FbxDouble3(1.0, 1.0, 1.0);
			}

			prop = material->FindProperty(factor_check_list[i]);
			if (prop.IsValid())
			{
				factors[i] = prop.Get<FbxDouble>();
			}
			else
			{
				factors[i] = 1.0;
			}
		}
		prop = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
		float alpha = prop.Get<FbxFloat>();

		if (alpha == 1)
		{
			alpha++;
		}
		else if (alpha >= 0.0f)
		{
			alpha = 0;
		}
	}

	FbxDouble3 color = colors[(int)MaterialOrder::Ambient];
	FbxDouble factor = factors[(int)MaterialOrder::Ambient];
	entry_material.SetAmbient((float)color[0], (float)color[1], (float)color[2], (float)factor);

	color = colors[(int)MaterialOrder::Diffuse];
	factor = factors[(int)MaterialOrder::Diffuse];
	entry_material.SetDiffuse((float)color[0], (float)color[1], (float)color[2], (float)factor);

	m_Materials[material->GetName()] = entry_material;

	// テクスチャ読み込み(シングル対応)
	// Diffuseプロパティを取得
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// propからFbxFileTextureを取得	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTextureからFbxFileTextureを取得	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (texture != nullptr &&
		LoadTexture(texture, keyword) == true)
	{
		// 読み込んだテクスチャとマテリアルの関係を覚えておく
		m_MaterialLinks[material->GetName()] = m_Textures[keyword];
	}
}

//VertexBuffer作成関数
bool FbxMeshFile::CreateVertexBuffer(ID3D11Device* device)
{
	for (auto& mesh : m_MeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(CustomVertex) * (UINT)mesh.m_Vertices.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
		buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.m_Vertices[0];	// バッファの中身の設定
		sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(device->CreateBuffer(
			&buffer_desc,								// バッファ情報
			&sub_resource,								// リソース情報
			&mesh.m_VertexBuffer)))	// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}

//IndexBuffer作成関数
bool FbxMeshFile::CreateIndexBuffer(ID3D11Device* device)
{
	for (auto& mesh : m_MeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(UINT) * (UINT)mesh.m_Indices.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND設定
		buffer_desc.CPUAccessFlags = 0;										// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;											// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;								// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.m_Indices[0];							// バッファの中身の設定
		sub_resource.SysMemPitch = 0;										// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;									// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(device->CreateBuffer(
			&buffer_desc,						// バッファ情報
			&sub_resource,						// リソース情報
			&mesh.m_IndexBuffer)))		// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}

//入力レイアウト作成関数
bool FbxMeshFile::CreateInputLayout(ID3D11Device* device, VertexShader* vertex_shader)
{
	D3D11_INPUT_ELEMENT_DESC vertex_desc[]{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT,   0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点レイアウト作成
	if (FAILED(device->CreateInputLayout(
		vertex_desc,				// レイアウト設定
		ARRAYSIZE(vertex_desc),		// 配列サイズ
		vertex_shader->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
		vertex_shader->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
		&m_InputLayout)))			// 作成された頂点レイアウトの格納先
	{
		return false;
	}

	return true;
}


//マテリアルデータ保存関数
void FbxMeshFile::SetMaterialColor(DirectGraphics* graphics, ObjMaterial& material)
{
	ConstantBuffer* constant = graphics->GetConstantBufferData();
	constant->MaterialDiffuse = DirectX::XMFLOAT4(material.Diffuse[0], material.Diffuse[1], material.Diffuse[2], material.Diffuse[3]);
	constant->MaterialAmbient = DirectX::XMFLOAT4(material.Ambient[0], material.Ambient[1], material.Ambient[2], material.Ambient[3]);
}

//モーション読み込み関数
bool FbxMeshFile::LoadMotion(std::string keyword_, const char* fileName_)
{
	// FbxManager作成
	FbxManager* fbx_manager = fbxsdk::FbxManager::Create();
	if (fbx_manager == nullptr)
	{
		return false;
	}

	// FbxImporter作成
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	if (fbx_importer == nullptr)
	{
		fbx_manager->Destroy();
		return false;
	}

	// FbxScene作成
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "");
	if (fbx_scene == nullptr)
	{
		fbx_importer->Destroy();
		fbx_manager->Destroy();
		return false;
	}

	FbxIOSettings* ios = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(ios);

	// FBXファイルを開いて、初期化する
	if (fbx_importer->Initialize(fileName_) == false) {
		// 読み込み失敗
		fbx_importer->Destroy();
		fbx_manager->Destroy();
		return false;
	}

	// ImporterでFbxデータを分解してSceneにインポートする
	if (fbx_importer->Import(fbx_scene) == false) {
		// インポート失敗
		fbx_importer->Destroy();
		fbx_manager->Destroy();
		return false;
	}

	// モーション情報取得
	FbxArray<FbxString*> names;
	fbx_scene->FillAnimStackNameArray(names);

	if (names == nullptr) return false;

	// モーションが存在するとき
	FbxTakeInfo* take = fbx_scene->GetTakeInfo(names[0]->Buffer());
	FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	m_StartFrame = static_cast<int>(start / fps60);
	m_Motion[keyword_].FrameNum = static_cast<UINT>((stop - start) / fps60);

	FbxNode* root = fbx_scene->GetRootNode();

	for (unsigned __int16 b = 0; b < m_BoneNum; b++)
	{
		// ボーンノードを検索
		FbxNode* bone = root->FindChild(m_Bone[b].Name);
		if (bone == nullptr)
		{
			continue;
		}

		// キーフレーム読み込み
		LoadKeyFrame(keyword_, b, bone);
	}

	return true;
}

//ボーンデータ読み込み関数
void FbxMeshFile::LoadBones(MeshData& meshData_, FbxMesh* mesh_)
{
	int skin_num = mesh_->GetDeformerCount(FbxDeformer::eSkin);
	if (skin_num <= 0) return;

	int vertex_num = (int)meshData_.m_Vertices.size();
	FbxSkin* skin = static_cast<FbxSkin*>(mesh_->GetDeformer(0, FbxDeformer::eSkin));

	// ボーン数を取得
	int bone_num = skin->GetClusterCount();
	// ボーンの取得
	for (int b = 0; b < bone_num; ++b)
	{
		FbxCluster* cluster = skin->GetCluster(b);
		FbxAMatrix trans;
		cluster->GetTransformMatrix(trans);
		trans.mData[0][1] *= -1;
		trans.mData[0][2] *= -1;
		trans.mData[1][0] *= -1;
		trans.mData[2][0] *= -1;
		trans.mData[3][0] *= -1;

		// ボーンの名前を取得
		const char* bone_name = cluster->GetLink()->GetInitialName();

		Bone* bone;
		int bone_no = FindBone(bone_name);
		if (bone_no != -1)
		{
			bone = &m_Bone[bone_no];
		}
		else
		{
			bone_no = m_BoneNum;
			bone = &m_Bone[bone_no];
			m_BoneNum++;

			strcpy_s(bone->Name, bone_name);

			// オフセット行列の作成
			FbxAMatrix link_mat;
			cluster->GetTransformLinkMatrix(link_mat);
			link_mat.mData[0][1] *= -1;
			link_mat.mData[0][2] *= -1;
			link_mat.mData[1][0] *= -1;
			link_mat.mData[2][0] *= -1;
			link_mat.mData[3][0] *= -1;

			FbxAMatrix offset = link_mat.Inverse() * trans;
			FbxDouble* offset_mat = (FbxDouble*)offset;

			bone->Offset = DirectX::XMMatrixSet(
				(float)offset_mat[0], (float)offset_mat[1], (float)offset_mat[2], (float)offset_mat[3],
				(float)offset_mat[4], (float)offset_mat[5], (float)offset_mat[6], (float)offset_mat[7],
				(float)offset_mat[8], (float)offset_mat[9], (float)offset_mat[10], (float)offset_mat[11],
				(float)offset_mat[12], (float)offset_mat[13], (float)offset_mat[14], (float)offset_mat[15]);
		}

		// ウェイトの読み込み
		int weight_num = cluster->GetControlPointIndicesCount();
		int* weight_indices = cluster->GetControlPointIndices();
		double* weight = cluster->GetControlPointWeights();

		int* index = mesh_->GetPolygonVertices();

		for (int i = 0; i < weight_num; ++i)
		{
			int weight_id = weight_indices[i];
			for (int vtx_i = 0; vtx_i < vertex_num; ++vtx_i)
			{
				// 頂点番号が一致しなければコンテニュー
				if (index[vtx_i] != weight_id) continue;

				int weight_count;
				for (weight_count = 0; weight_count < 4; ++weight_count)
				{
					if (meshData_.m_Vertices[vtx_i].Weight[weight_count] <= 0.0f) break;
				}
				if (4 <= weight_count) continue;

				meshData_.m_Vertices[vtx_i].Index[weight_count] = bone_no;
				meshData_.m_Vertices[vtx_i].Weight[weight_count] = static_cast<float>(weight[i]);
			}
		}
	}

	// ウェイト正規化
	for (int vtx_i = 0; vtx_i < vertex_num; ++vtx_i)
	{
		float n = 0;

		int weight_count;
		for (weight_count = 0; weight_count < 4; weight_count++)
		{
			if (meshData_.m_Vertices[vtx_i].Weight[weight_count] <= 0.0f) break;

			n += meshData_.m_Vertices[vtx_i].Weight[weight_count];
		}

		for (weight_count = 0; weight_count < 4; weight_count++)
		{
			meshData_.m_Vertices[vtx_i].Weight[weight_count] /= n;
		}
	}
}

//ボーン検索関数
int FbxMeshFile::FindBone(const char* boneName_)
{
	for (UINT i = 0; i < m_BoneNum; i++)
	{
		if (strcmp(m_Bone[i].Name, boneName_) == 0)
		{
			return (int)i;
		}
	}
	return -1;
}

//キーフレーム読み込み関数
void FbxMeshFile::LoadKeyFrame(std::string keyword_, int bone_, FbxNode* boneNode_)
{
	Motion* motion = &m_Motion[keyword_];
	motion->Key[bone_].resize(motion->FrameNum);

	double time = static_cast<double>(m_StartFrame * (1.0 / 60.0));
	FbxTime T;
	for (UINT f = 0; f < motion->FrameNum; ++f)
	{
		T.SetSecondDouble(time);

		FbxMatrix m = boneNode_->EvaluateGlobalTransform(T);
		m.mData[0][1] *= -1; // _12
		m.mData[0][2] *= -1; // _13
		m.mData[1][0] *= -1; // _21
		m.mData[2][0] *= -1; // _31
		m.mData[3][0] *= -1; // _41

		FbxDouble* mat = static_cast<FbxDouble*>(m);
		motion->Key[bone_][f] = DirectX::XMMatrixSet(
			(float)mat[0], (float)mat[1], (float)mat[2], (float)mat[3],
			(float)mat[4], (float)mat[5], (float)mat[6], (float)mat[7],
			(float)mat[8], (float)mat[9], (float)mat[10], (float)mat[11],
			(float)mat[12], (float)mat[13], (float)mat[14], (float)mat[15]
		);
		time += 1.0 / 60.0;
	}

}

//モーション指定関数
void FbxMeshFile::Play(std::string animName_)
{
	if (m_PlayMotion != animName_)
	{
		m_Frame = 0.0f;
	}

	m_PlayMotion = animName_;
}

//アニメーション関数
void FbxMeshFile::Animate()
{
	m_Frame += 1.0f;
	if (m_Frame >= m_Motion[m_PlayMotion].FrameNum - 1)
	{
		m_Frame = 2.0f;
	}
}

