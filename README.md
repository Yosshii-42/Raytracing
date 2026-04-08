## 実行

- main()の中でシーンを選択する(49~53)
- make
- ./raytracing > image.ppm
- image.ppmファイルを開く

## 構成

- **Vec3**  
  ベクトル計算（加算・減算・内積・外積など）
- **Ray**  
  レイ定義（始点と方向ベクトル）
- **Hittable**  
  衝突判定のインターフェース
- **Camera**  
  カメラ操作（視点・視線・レイ生成）
- **Material**  
  マテリアル（マット、金属、透明、発光体）
- **Texture**  
  テクスチャ（ソリッドカラー、床用チェッカー、球用チェッカー、画像テクスチャ）
- **Color**  
  色の出力・補正処理
- **Utils**  
  補助関数（乱数、clamp など）
- **Sphere**  
  球の定義と交差判定
- **Rectangle**  
  長方形の定義と交差判定
- **AABB**  
  直方体を表すクラス
- **World**  
  シーンリスト

## 参考資料

- https://inzkyk.xyz/ray_tracing_in_one_weekend/
