# Action_Game
就職作品_DirectXで制作

動作環境
Windows8  移行
DirectX11 以上対応

.hlsl のエントリーポイントは必ず main で
名前の最初にVS_ PS_ CS_ とつけること


最適化可能情報
デシリアライズで String_View から String にしてから cast しています。
DirectX_Math Struct 内で 自作Struct メンバー関数を使用しているため
DirectXStruct と自作構造体の 不必要な変換が行われておる。