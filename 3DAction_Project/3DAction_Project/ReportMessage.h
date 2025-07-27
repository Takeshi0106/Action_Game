#pragma once

namespace ErrorLog {
bool IsHRESULTFailedWithLog(long hr, const char* message); // HRESULTとメッセージボックスで出力するメッセージを引き数で渡す　失敗時に戻り値はtrueを返す

}