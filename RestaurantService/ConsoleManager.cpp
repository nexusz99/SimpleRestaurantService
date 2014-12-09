#include "ConsoleLogger.h"

CConsoleLoggerEx orderConsole;
CConsoleLoggerEx monitorConsole;

void initConsole()
{
	orderConsole.Create("주문 현황판");
	monitorConsole.Create("관리자 화면");
}