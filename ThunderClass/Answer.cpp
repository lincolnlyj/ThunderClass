﻿/*************************************************************************
【文件名】                 Answer.cpp
【功能模块和目的】         Answer类函数的定义
【开发者及日期】           李永健 2020/6/18
【更改记录】               无
*************************************************************************/
#include "Answer.h"

/*************************************************************************
【函数名称】       构造函数
【函数功能】       类内数据初始化
【参数】           无
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
Answer::Answer()
	: AswAndTime(m_cAswAndTime)
{
	for (int i = 0; i < 9; i++)
	{
		m_cAswAndTime[i] = 0;
	}
}

/*************************************************************************
【函数名称】       构造函数
【函数功能】       类内数据初始化
【参数】           答案和时间
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
Answer::Answer(const string& MyAnswer, int MyTime)
	: AswAndTime(m_cAswAndTime)
{
	SetTime(MyTime);
	SetAnswer(MyAnswer);
}

/*************************************************************************
【函数名称】       构造函数
【函数功能】       类内数据初始化
【参数】           数据指针
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
Answer::Answer(const char* pcData)
	: AswAndTime(m_cAswAndTime)
{
	for (int i = 0; i < 9; i++)
	{
		m_cAswAndTime[i] = pcData[i];
	}
}

/*************************************************************************
【函数名称】       拷贝构造函数
【函数功能】       初始化时对象的拷贝
【参数】           Answer类对象的常引用
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
Answer::Answer(const Answer& data)
	: AswAndTime(m_cAswAndTime)
{
	for (int i = 0; i < 9; i++)
	{
		m_cAswAndTime[i] = data.m_cAswAndTime[i];
	}
}

/*************************************************************************
【函数名称】       析构函数
【函数功能】       对象清理
【参数】           无
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
Answer::~Answer()
{

}

/*************************************************************************
【函数名称】       赋值运算符重载
【函数功能】       对象的赋值
【参数】           Answer类对象的常引用
【返回值】         this指针指向的对象
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
Answer& Answer::operator=(const Answer& data)
{
	if (&data != this)
	{
		for (int i = 0; i < 9; i++)
		{
			m_cAswAndTime[i] = data.m_cAswAndTime[i];
		}
	}
	return *this;
}

/*************************************************************************
【函数名称】       SetAnswer
【函数功能】       设置答案
【参数】           传入的答案
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
void Answer::SetAnswer(const string& MyAnswer)
{
	for (int i = 4; i < 9; i++)
	{
		m_cAswAndTime[i] = 0;
	}
	for (int i = 0; i < MyAnswer.length(); i++)
	{
		m_cAswAndTime[i + 4] = MyAnswer[i];
	}
}

/*************************************************************************
【函数名称】       SetTime
【函数功能】       设置时长
【参数】           时长
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
void Answer::SetTime(int MyTime)
{
	int* piTime = (int*)m_cAswAndTime;
	*piTime = MyTime;
}

/*************************************************************************
【函数名称】       GetAnswer
【函数功能】       获取答案
【参数】           无
【返回值】         string类型的答案
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
string Answer::GetAnswer() const
{
	return string(m_cAswAndTime + 4);
}

/*************************************************************************
【函数名称】       GetTime
【函数功能】       获取时长
【参数】           无
【返回值】         时长
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
int Answer::GetTime() const
{
	return *((int*)m_cAswAndTime);
}

/*************************************************************************
【函数名称】       GetHour
【函数功能】       获取小时数
【参数】           无
【返回值】         小时数
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
int Answer::GetHour() const
{
	return *((int*)m_cAswAndTime) / 1000 / 3600;
}

/*************************************************************************
【函数名称】       GetMin
【函数功能】       获取分钟数
【参数】           无
【返回值】         分钟数
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
int Answer::GetMin() const
{
	return ((*((int*)m_cAswAndTime) / 1000) % 3600) / 60;
}

/*************************************************************************
【函数名称】       GetSec
【函数功能】       获取秒数
【参数】           无
【返回值】         秒数
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
int Answer::GetSec() const
{
	return (*((int*)m_cAswAndTime) / 1000) % 60;
}

/*************************************************************************
【函数名称】       Check
【函数功能】       判断选项是否选择
【参数】           选项
【返回值】         bool类型
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
bool Answer::Check(char Option) const
{
	if (GetAnswer().find(Option) == -1)//如果没有找到Option就返回false
	{
		return false;
	}
	else
	{
		return true;
	}
}