#include "ConsoleLogger.h"

CConsoleLoggerEx orderConsole;
CConsoleLoggerEx monitorConsole;

void initConsole()
{
	orderConsole.Create("�ֹ� ��Ȳ��");
	monitorConsole.Create("������ ȭ��");
}