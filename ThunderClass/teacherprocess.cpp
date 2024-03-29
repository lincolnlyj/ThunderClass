﻿/*************************************************************************
【文件名】teacherprocess.cpp
【功能模块和目的】教师业务流程类定义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】
	2020-05-19 由范静涛修改为信号机制通知界面更新
	2020/6/17 由李永健增加收注意力信息的机制
	2020/6/19 由李永健增加了避免中文乱码的机制
	2020/6/19 由李永健增加对于随机提问的适配
	2020/6/20 由李永健增加输出到文件的功能
*************************************************************************/
#include "teacherprocess.h"
#include <string>
#include "user.h"
#include "student.h"
#include "QStrAndStr.h"
#include <QDebug>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

/*************************************************************************
【函数名称】TeacherProcess::TeacherProcess
【函数功能】构造函数
【参数】入口参数，父对象指针，默认为nullptr
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
	2020-05-19 由范静涛删除了界面类控件指针，修改为信号机制通知界面更新
	2020/6/19 由李永健增加对于随机提问和答题的适配
*************************************************************************/
TeacherProcess::TeacherProcess(QObject* Parent)
	: QObject(Parent), IsInClass(m_bIsInClass),
	IsScreenSharing(m_bIsScreenSharing), IsVoiceSharing(m_bIsVoiceSharing) {
	//新建Server
	m_pServer = new Server();
	//新建录音对象
	m_pVoice = new Audio();
	//更新为未在上课状态
	m_bIsInClass = false;
	//更新为未在屏幕共享状态
	m_bIsScreenSharing = false;
	//更新为未在语音直播状态
	m_bIsVoiceSharing = false;
	//未等待接受答案
	m_bRecAnswer = false;
	//未语音提问
	m_uRandCallIdx = -1;
	//初始化所有线程句柄为空
	m_hScreenShareThread = nullptr;
	m_hVoiceShareThread = nullptr;
	m_hMsgReceiveThread = nullptr;
	//初始化三个数组，其实不必要，怕后面写忘了，放在这里作提示
	m_Names.clear();
	m_Passwords.clear();
	m_ConnectionIdx.clear();
	qDebug() << "TP: Created";
}

/*************************************************************************
【函数名称】TeacherProcess::~TeacherProcess
【函数功能】析构函数
【参数】无
【返回值】析构函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
TeacherProcess::~TeacherProcess() {
	//下课
	EndClass();
	//释放动态开辟的对象
	delete m_pServer;
	m_pServer = nullptr;
	delete m_pVoice;
	m_pVoice = nullptr;
}

/*************************************************************************
【函数名称】TeacherProcess::BeginClass
【函数功能】开始上课
【参数】无
【返回值】true为当前正在上课
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
	2020-05-19 由范静涛修改为信号机制通知界面更新
	2020/6/19 由李永健增加对于随机提问和答题的适配
*************************************************************************/
bool TeacherProcess::BeginClass() {
	//已是上课状态，什么都不做
	if (m_bIsInClass) {
		return m_bIsInClass;
	}
	//清空三个数组，相当于没有人登录了
	m_Names.clear();
	m_Passwords.clear();
	m_ConnectionIdx.clear();
	m_Answers.clear();
	//所有学生对象下线，之前登录状态都不算
	User::OfflineAllStudents();
	//打开服务端
	qDebug() << "TP: open server";
	m_pServer->Open(7908);
	//更新状态为上课中
	m_bIsInClass = true;
	//未等待接受答案
	m_bRecAnswer = false;
	//未语音提问
	m_uRandCallIdx = -1;
	//创建并启动消息接收与处理线程
	m_hMsgReceiveThread = CreateThread(nullptr, 0, MsgReceiveThread, (LPVOID)this, 0, nullptr);
	if (m_hMsgReceiveThread == nullptr) {
		qDebug() << "TP: failed to create a MsgRecv&Process thread";
		throw(runtime_error("Failed to create a MsgRecv&Process thread!"));
	}
	//发送开始上课信号，要求界面更新
	qDebug() << "TP: class begin";
	emit ClassBegined();
	return m_bIsInClass;
}

/*************************************************************************
【函数名称】TeacherProcess::EndClass
【函数功能】下课
【参数】无
【返回值】true为当前正在上课（虽然是下课功能，但返回值为真时正在上课）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
	2020-05-19 由范静涛修改为信号机制通知界面更新
	2020/6/17 由李永健增加接收注意力信息的机制
	2020/6/19 由李永健增加对于随机提问和答题的适配
*************************************************************************/
bool TeacherProcess::EndClass() {
	//已下课，则检查消息接收与处理线程是否自主退出了
	if (!m_bIsInClass) {
		if (m_hMsgReceiveThread != nullptr) {
			qDebug() << "TP: clean MsgRecv&Process thread handle";
			CloseHandle(m_hMsgReceiveThread);
			m_hMsgReceiveThread = nullptr;
		}
		return m_bIsInClass;
	}
	//停止可能正在的屏幕共享
	StopScreenShare();
	//停止可能正在的语音直播
	StopVoiceShare();
	//关闭服务端
	qDebug() << "TP: server close";
	m_pServer->Close();
	//更新未在上课状态
	m_bIsInClass = false;
	//未等待接受答案
	m_bRecAnswer = false;
	//未语音提问
	m_uRandCallIdx = -1;
	//等待消息接收和处理线程结束
	qDebug() << "TP: wait for MsgRecv&Process thread exit";
	WaitForSingleObject(m_hMsgReceiveThread, 2000);
	qDebug() << "TP: clean MsgRecv&Process thread handle";
	CloseHandle(m_hMsgReceiveThread);
	m_hMsgReceiveThread = nullptr;

	pair<unsigned int, Message> TempPair;//将所有时间统计数据提取出来
	while (m_pServer->GetAndRemoveFirstMessage(TempPair))
	{
		if (TempPair.second.Type() == MSG_TIMECOUNT)
		{
			int iPos;//对应的下标
			for (iPos = 0; iPos < m_ConnectionIdx.size(); iPos++)
			{
				if (m_ConnectionIdx[iPos] == TempPair.first)
				{
					break;//找到对应的学生
				}
			}
			Student* pStu =
				const_cast<Student*>(dynamic_cast<const Student*>
				(User::GetUser(m_Names[iPos], m_Passwords[iPos])));
			pStu->SaveTimeCount(Message::ToTimeCount(TempPair.second));
		}
	}

	//所有人下线，重置状态
	m_Names.clear();
	m_Passwords.clear();
	m_ConnectionIdx.clear();
	m_Answers.clear();
	qDebug() << "TP: offline all students";
	User::OfflineAllStudents();

	//发送下课信号，要求界面更新
	qDebug() << "TP: class end";
	emit ClassEnded();

	return m_bIsInClass;
}

/*************************************************************************
【函数名称】TeacherProcess::StartScreenShare
【函数功能】开始屏幕共享
【参数】无
【返回值】true为当前正在屏幕共享
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool TeacherProcess::StartScreenShare() {
	if (!m_bIsInClass) {
		return m_bIsInClass;
	}
	if (m_bIsScreenSharing) {
		return m_bIsScreenSharing;
	}
	m_hScreenShareThread = CreateThread(nullptr, 0, ScreenShareThread, (LPVOID)this, 0, nullptr);
	if (m_hScreenShareThread == nullptr) {
		qDebug() << "TP: failed to create a new screen share thread";
		throw(runtime_error("Failed to create a new screen share thread!"));
	}
	m_bIsScreenSharing = true;
	qDebug() << "TP: start screen share";
	return m_bIsScreenSharing;
}

/*************************************************************************
【函数名称】TeacherProcess::StopScreenShare
【函数功能】停止屏幕共享
【参数】无
【返回值】true为当前正在屏幕共享
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool TeacherProcess::StopScreenShare() {
	if (!m_bIsScreenSharing) {
		return m_bIsScreenSharing;
	}
	if (!m_bIsInClass) {
		return m_bIsInClass;
	}
	m_bIsScreenSharing = false;
	qDebug() << "TP: wait for screen share thread exit";
	WaitForSingleObject(m_hScreenShareThread, 2000);
	qDebug() << "TP: clean screen share thread handle";
	CloseHandle(m_hScreenShareThread);
	m_hScreenShareThread = nullptr;
	qDebug() << "TP: stop screen share";
	return m_bIsScreenSharing;
}

/*************************************************************************
【函数名称】TeacherProcess::StartVoiceShare
【函数功能】开始语音直播
【参数】无
【返回值】true为当前正在语音直播
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool TeacherProcess::StartVoiceShare() {
	if (!m_bIsInClass) {
		return m_bIsInClass;
	}
	if (m_bIsVoiceSharing) {
		return m_bIsVoiceSharing;
	}
	m_hVoiceShareThread = CreateThread(nullptr, 0, VoiceShareThread, (LPVOID)this, 0, nullptr);
	if (m_hVoiceShareThread == nullptr) {
		qDebug() << "TP: failed to create a new voice share thread";
		throw(runtime_error("Failed to create a new voice share thread!"));
	}
	m_pVoice->StartRec();
	m_bIsVoiceSharing = true;
	qDebug() << "TP: start voice share";
	return m_bIsVoiceSharing;
}

/*************************************************************************
【函数名称】TeacherProcess::StopVoiceShare
【函数功能】停止语音直播
【参数】无
【返回值】true为当前正在语音直播
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool TeacherProcess::StopVoiceShare() {
	if (!m_bIsVoiceSharing) {
		return m_bIsVoiceSharing;
	}
	if (!m_bIsInClass) {
		return m_bIsInClass;
	}
	m_pVoice->StopRec();
	m_bIsVoiceSharing = false;
	qDebug() << "TP: wait for voice share thread exit";
	WaitForSingleObject(m_hVoiceShareThread, 2000);

	CloseHandle(m_hVoiceShareThread);
	m_hVoiceShareThread = nullptr;
	qDebug() << "TP: clean voice share thread handel";
	AudioFrame TempFrame;
	while (m_pVoice->RecordFramesCount() > 0) {
		m_pVoice->GetAndRemoveFirstRecorded(TempFrame);
	}
	qDebug() << "TP: clean voice share queue";
	qDebug() << "TP: stop voice share";
	return m_bIsVoiceSharing;
}

/*************************************************************************
【函数名称】TeacherProcess::SendStrChat
【函数功能】群发消息
【参数】入口参数，消息内容QString
【返回值】是否发送成功
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】2020/6/19 由李永健增加了避免中文乱码的机制
*************************************************************************/
bool TeacherProcess::SendStrChat(const QString& ChatStr) {
	if (!m_bIsInClass) {
		return m_bIsInClass;
	}
	std::string ToSend = QStrToStr(ChatStr);
	Message Chat(MSG_STRING, (unsigned char*)ToSend.c_str(), ToSend.length());
	m_pServer->SendToAll(Chat);
	qDebug() << "TP: send to all message: " << ChatStr;
	return m_bIsInClass;
}

/*************************************************************************
【函数名称】       SendQuestion
【函数功能】       发送题目
【参数】           题目和选项以及题目类型
【返回值】         无
【开发者及日期】   李永健 2020/6/18
【更改记录】       2020/6/19 由李永健增加了避免中文乱码的机制
*************************************************************************/
void TeacherProcess::SendQuestion(const QString& MyQuestion, const QString& OptionA,
	const QString& OptionB, const QString& OptionC, const QString& OptionD, QuestionType Type)
{
	Question TheQuestion(QStrToStr(MyQuestion), QStrToStr(OptionA), QStrToStr(OptionB),
		QStrToStr(OptionC), QStrToStr(OptionD), Type);
	m_pServer->SendToAll(Message::FromQuestion(TheQuestion));
	m_bRecAnswer = true;//开始等待答案
}

/*************************************************************************
【函数名称】       GetAnswer
【函数功能】       收题
【参数】           无
【返回值】         答案和姓名的vector
【开发者及日期】   李永健 2020/6/18
【更改记录】       无
*************************************************************************/
vector<pair<string, Answer> > TeacherProcess::GetAnswer()
{
	vector<pair<string, Answer> > TempData;//存放答题信息的临时数组
	m_pServer->SendToAll(Message(MSG_ENDQUES, nullptr, 0));//发送停止答题消息
	Sleep(200);//休眠0.2秒，等待答案
	m_bRecAnswer = false;
	m_AnswerLock.lock();
	for (int i = 0; i < m_Names.size(); i++)
	{
		if (m_Answers[i].GetTime() != 0)//如果有答题信息，就存入临时数组
		{
			TempData.push_back(pair<string, Answer>(m_Names[i], m_Answers[i]));
			m_Answers[i] = Answer();//清空消息
		}
	}
	m_AnswerLock.unlock();
	return TempData;
}

/*************************************************************************
【函数名称】       GetTimeCount
【函数功能】       获取学生专注信息
【参数】           无
【返回值】         学生专注信息的数组
【开发者及日期】   李永健 2020/6/18
【更改记录】       
	2020/6/20 由李永健增加输出到文件的功能
*************************************************************************/
vector<pair<string, const TimeCount> > TeacherProcess::GetTimeCount() const
{
	vector<pair<string, const TimeCount> > CTempData;
	ofstream fout("ClassData.txt");
	for (int i = 0; i < User::UserCount; i++)
	{
		const User* pUser = User::GetUser(i);
		if (pUser->Type == "Student")//如果是学生
		{
			Student* pStu = const_cast<Student*>(dynamic_cast<const Student*>(pUser));
			if (pStu->TheTimeCount.Data[0] != 0)//如果学生的专注数据不是0
			{
				CTempData.push_back(
					pair<string, const TimeCount>(pStu->GetName(), pStu->TheTimeCount)
				);
				fout << pStu->GetName() << endl;
				fout << "专注时长: " << pStu->TheTimeCount.GetHourFocused() << "h"
					<< pStu->TheTimeCount.GetMinFocused() << "min"
					<< pStu->TheTimeCount.GetSecFocused() << "s" << endl;
				fout << "在线时长: " << pStu->TheTimeCount.GetHourInClass() << "h"
					<< pStu->TheTimeCount.GetMinInClass() << "min"
					<< pStu->TheTimeCount.GetSecInClass() << "s" << endl;
				fout << "专注百分比: " << pStu->TheTimeCount.GetFocusedRate() << "%" << endl;
			}
			pStu->ClearTimeCount();//清空学生专注数据
		}
	}
	fout.close();
	return CTempData;
}

/*************************************************************************
【函数名称】       RandCall
【函数功能】       发送语音提问消息
【参数】           无
【返回值】         无
【开发者及日期】   李永健 2020/6/19
【更改记录】       无
*************************************************************************/
bool TeacherProcess::RandCall()
{
	if (m_ConnectionIdx.empty())
	{
		return false;
	}
	srand(time(nullptr));//随机选取在线学生
	int iNum = rand() % m_ConnectionIdx.size();
	m_uRandCallIdx = m_ConnectionIdx[iNum];
	emit SelectedStudent(StrToQStr(m_Names[iNum]));
	m_pServer->SendTo(m_uRandCallIdx, Message(MSG_OPENAUDIO, nullptr, 0));//发送随机答题消息
	m_pVoice->StartPlay();//开始播放
	return true;
}

/*************************************************************************
【函数名称】       StopRandCall
【函数功能】       停止语音提问
【参数】           无
【返回值】         无
【开发者及日期】   李永健 2020/6/19
【更改记录】       无
*************************************************************************/
bool TeacherProcess::StopRandCall()
{
	if (m_uRandCallIdx == -1)
	{
		return false;
	}
	m_pServer->SendTo(m_uRandCallIdx, Message(MSG_CLOSAUDIO, nullptr, 0));
	m_pVoice->StopPlay();//停止播放
	m_uRandCallIdx = -1;//停止随机提问
	return true;
}

/*************************************************************************
【函数名称】TeacherProcess::ScreenShareThread
【函数功能】屏幕共享线程函数
【参数】入口参数，TeacherProcess的指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
DWORD WINAPI TeacherProcess::ScreenShareThread(LPVOID lpParameter) {
	TeacherProcess* pProcess = (TeacherProcess*)lpParameter;
	Image TempImage;
	while (pProcess->IsScreenSharing) {
		//如果平均未发消息不足10，就抓屏幕，转换成jpg消息，从server群发出去
		if (pProcess->m_pServer->GetAvgToSendCount() <= 10) {
			CapScreenTo(TempImage);
			pProcess->m_pServer->SendToAll(Message::FromImageJpg(TempImage));
			qDebug() << "TP: send to all screen";
		}
		//限定帧率：两次抓屏间隔不小于40ms（1/24秒）
		Sleep(40);
	}
	return 0;
}

/*************************************************************************
【函数名称】TeacherProcess::VoiceShareThread
【函数功能】语音线程函数
【参数】入口参数，TeacherProcess的指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
	2020-05-19 范静涛优化了代码效率，每次都取出一个音频帧，不管是否发送，都
*************************************************************************/
DWORD WINAPI TeacherProcess::VoiceShareThread(LPVOID lpParameter) {
	TeacherProcess* pProcess = (TeacherProcess*)lpParameter;
	AudioFrame TempFrame;
	while (pProcess->IsVoiceSharing) {
		//取出一个录制好的音频帧
		if (pProcess->m_pVoice->GetAndRemoveFirstRecorded(TempFrame)) {
			qDebug() << "TP: remove an audio frame";
			//如果平均未发消息不足10，就抓屏幕，转换成音频消息，从server群发出去
			if (pProcess->m_pServer->GetAvgToSendCount() <= 10) {
				pProcess->m_pServer->SendToAll(Message::FromAudio(TempFrame));
				qDebug() << "TP: send to all an audio frame";
			}
		}
	}
	return 0;
}

/*************************************************************************
【函数名称】TeacherProcess::MsgRecieveThread
【函数功能】消息接收和处理线程函数
【参数】入口参数，TeacherProcess的指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
	2020-05-19 由范静涛修改为信号机制通知界面更新
	2020/6/17 由李永健增加收注意力信息的机制
	2020/6/19 由李永健增加了避免中文乱码的机制
*************************************************************************/
DWORD WINAPI TeacherProcess::MsgReceiveThread(LPVOID lpParameter) {
	TeacherProcess* pProcess = (TeacherProcess*)lpParameter;
	//临时消息pair，两个分量分别从哪个连接收到的消息、收到了什么消息
	pair<unsigned int, Message> TempPair;
	MessageType Type;
	while (pProcess->IsInClass) {
		//如果有待处理消息，则取出最早的一个
		if (pProcess->m_pServer->GetAndRemoveFirstMessage(TempPair)) {
			//获取消息类型
			Type = TempPair.second.Type();
			//如果是文字信息，就通知界面更新
			if (Type == MSG_STRING) {
				qDebug() << "TP: recieved s string from connection " << TempPair.first;
				emit pProcess->RecvChat(StrToQStr(Message::ToString(TempPair.second)));
			}
			//如果是用户名密码的登录消息
			else if (Type == MSG_ID_PSW) {
				//这个学生已经登录了并正常通信中，则不允许重复登录；如果未登录过，但用户名密码不对，也是一样
				if (!pProcess->OnlineStudent(TempPair)) {
					//断开连接
					qDebug() << "TP: disconnect socket: " << TempPair.first;
					pProcess->m_pServer->DisConnect(TempPair.first);
				}
			}
			else if (Type == MSG_ANSWER && pProcess->m_bRecAnswer)//如果是答案消息，且答案消息
			{
				pProcess->m_AnswerLock.lock();
				int iPos;//对应的下标
				for (iPos = 0; iPos < pProcess->m_ConnectionIdx.size(); iPos++)
				{
					if (pProcess->m_ConnectionIdx[iPos] == TempPair.first)
					{
						break;//找到对应的学生
					}
				}
				pProcess->m_Answers[iPos] = Message::ToAnswer(TempPair.second);
				pProcess->m_AnswerLock.unlock();
			}
			//如果是音频消息且正在随机提问
			else if (Type == MSG_AUDIO && pProcess->m_uRandCallIdx != -1)
			{
				pProcess->m_pVoice->AppendFrameToPlay(Message::ToAudioFrame(TempPair.second));//增加到播放列表
				pProcess->m_pServer->SendToExcept(pProcess->m_uRandCallIdx, TempPair.second);//发送给除了该学生以外的学生
			}
			//如果是退出消息，修改这个同学的登录状态
			else if (Type == MSG_EXIT) {
				pProcess->OffLineStudent(TempPair.first);
				pProcess->m_pServer->DisConnect(TempPair.first);
			}
			else if (Type == MSG_TIMECOUNT)
			{
				int iPos;//对应的下标
				for (iPos = 0; iPos < pProcess->m_ConnectionIdx.size(); iPos++)
				{
					if (pProcess->m_ConnectionIdx[iPos] == TempPair.first)
					{
						break;//找到对应的学生
					}
				}
				Student* pStu = 
					const_cast<Student*>(dynamic_cast<const Student*>
					(User::GetUser(pProcess->m_Names[iPos], pProcess->m_Passwords[iPos])));
				pStu->SaveTimeCount(Message::ToTimeCount(TempPair.second));
			}
		}
	}
	return 0;
}


//以下两个函数，不需要掌握
bool TeacherProcess::OnlineStudent(const pair<unsigned int, Message>& Pair) {
	string Name;
	string Password;
	//获得要登录的用户在哪个Connection
	unsigned int ConnectionIdx = Pair.first;
	//获得用户名密码
	Message::ToStudentNameAndPassword(Pair.second, Name, Password);
	//在用户列表中按用户名密码搜索用户
	qDebug() << "TP: " << StrToQStr(Name) << "is trying to login";
	const User* pCurrent = User::GetUser(Name, Password);
	//未找到此用户，返回false
	if (pCurrent == nullptr) {
		qDebug() << "TP: no user " << StrToQStr(Name);
		return false;
	}
	//找到用户则转换为Student指针
	Student* pStudent = const_cast<Student*>(dynamic_cast<const Student*>(pCurrent));
	int CurrentConnectionIdx = 0;
	//判定指定学生用户当前是否在线，如在线，在哪个connection
	if (pStudent->IsOnline(CurrentConnectionIdx)) {
		//学生在线，在CurrentConnectionIdx
		//判定CurrentConnectionIdx对应的连接是否处于通信状态
		if (m_pServer->IsConnected(CurrentConnectionIdx)) {
			qDebug() << "TP: " << StrToQStr(Name) << "has logined at " << CurrentConnectionIdx;
			//在通信状态，未断开，返回false，不允许重复登录
			return false;
		}
		//学生在线, 但connection已经断开了，更新
		else {
			//设定在线
			qDebug() << "TP: " << StrToQStr(Name) << " is reset to " << CurrentConnectionIdx;
			pStudent->Online(ConnectionIdx);
			//更新之前记录的ConnectionIdx
			for (unsigned int i = 0; i < m_Names.size(); i++) {
				if (m_Names[i] == Name && m_Passwords[i] == Password) {
					m_ConnectionIdx[i] = ConnectionIdx;
					break;
				}
			}
			//发出信号，让界面更新学生列表
			emit RecvStudentLogin(StrToQStr(Name));
			return true;
		}
	}
	//学生不在线
	else {
		//设定在吸纳
		qDebug() << "TP: " << StrToQStr(Name) << " login is ok. at " << CurrentConnectionIdx;
		pStudent->Online(ConnectionIdx);
		//记录用户名密码和ConnectionIdx到数组
		m_AnswerLock.lock();
		m_Names.push_back(Name);
		m_Passwords.push_back(Password);
		m_ConnectionIdx.push_back(ConnectionIdx);
		m_Answers.push_back(Answer());
		m_AnswerLock.unlock();
		//发出信号，让界面更新学生列表
		emit RecvStudentLogin(StrToQStr(Name));
		return true;
	}
}

void TeacherProcess::OffLineStudent(unsigned int Idx) {
	//搜索idx号connection对应的学生对象
	for (unsigned int i = 0; i < m_Names.size(); i++) {
		//搜索idx对相应的数组记录，获取用户名密码
		if (m_ConnectionIdx[i] == Idx) {
			//按用户名密码找学生对象指针
			const User* pCurrent = User::GetUser(m_Names[i], m_Passwords[i]);
			if (pCurrent == nullptr) {
				return;
			}
			Student* pStudent = const_cast<Student*>(dynamic_cast<const Student*>(pCurrent));
			//下线/登出学生对象
			qDebug() << "TP: " << StrToQStr(m_Names[i]) << " logout";
			pStudent->Offline();
			//发出信号，还要求更新界面显示
			emit RecvStudentLogout(StrToQStr(m_Names[i]));
			m_AnswerLock.lock();
			//删连接记录
			m_ConnectionIdx.erase(m_ConnectionIdx.begin() + i);
			//删用户名记录
			m_Names.erase(m_Names.begin() + i);
			//删密码记录
			m_Passwords.erase(m_Passwords.begin() + i);
			//删除答题信息
			m_Answers.erase(m_Answers.begin() + i);
			m_AnswerLock.unlock();
			break;
		}
	}
}