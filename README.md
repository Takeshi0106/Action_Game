# Action_Game

==== 注意事項 ===
・https://github.com/Takeshi0106/DirectXTex-Prebuilt/releases/download/2019/DirectXTex_2019.zip
・https://github.com/Takeshi0106/Assimp-Prebuilt/releases/download/5.2.5/5.2.5.zip
・https://github.com/Takeshi0106/DXC-Prebuilt/releases/download/v1.0/dxc_2026_0220.zip
　ビルドを行うと上が自動的にダウンロードされます。注意してください。


就職作品 
複数のプラットフォームでもGameMainを修正せずに動かせるように作成中。
今対応中のプラットフォームは DirectXだけです。
DirectXで描画などを対応できた後、GameMainを作成予定です。
現在は C++20 で作成しています。
(理由 ファイルシステム、コールチンを使用したいため)

動作環境
・ Windows8  移行
・ DirectX11 以上対応

開発環境
https://github.com/microsoft/DirectXShaderCompiler/releases?utm_source=chatgpt.com
上のURLの 2025年5月パッチの dxc.2025_07_14.zip を取得して、Main.cpp 内のDXC＿Pathを設定してください


ルール
・ .hlsl のエントリーポイントは必ず main で
    名前の最初にVS_ PS_ CS_ とつける。

最適化可能情報
/* -- 上から最適化が簡単で大幅に最適化できる順で記載すること -- */
・ 各マネージャーが管理している unordered_map が resize などの領域確保を行っていないため、
　 不必要にコピー領域を取得している。

・ デシリアライズで String_View から String にしてから cast しています。

・ 頂点シェーダーで作成している入力レイアウトは、頂点シェーダーごとに作成しているため、最適化できます。

・ DirectX_Math Struct 内で 自作Struct メンバー関数を使用しているため
    DirectXStruct と自作構造体の 不必要な変換が行われています。
