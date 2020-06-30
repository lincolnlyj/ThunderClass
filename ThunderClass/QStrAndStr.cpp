/*************************************************************************
【文件名】                 QStrAndStr.cpp
【功能模块和目的】         QStrAndStr.h中代码实现
【开发者及日期】           李永健 2020/6/19
【更改记录】               无
*************************************************************************/
#include "QStrAndStr.h"

/*************************************************************************
【函数名称】       QStrToStr
【函数功能】       从QString转成string
【参数】           QString
【返回值】         string
【开发者及日期】   李永健 2020/6/19
【更改记录】       无
*************************************************************************/
string QStrToStr(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}

/*************************************************************************
【函数名称】       StrToQStr
【函数功能】       从string转成QString
【参数】           string
【返回值】         QString
【开发者及日期】   李永健 2020/6/19
【更改记录】       无
*************************************************************************/
QString StrToQStr(const string str)
{
	return QString::fromLocal8Bit(str.data());
}