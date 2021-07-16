#pragma once

#include <QObject>
#include <QThreadPool>

#include "SProcess.h"
#include "SMemoryBuffer.h"

class SAbstractAction;

//
//
//
class SxyEngine : public QObject
{
	Q_OBJECT

public:
	~SxyEngine();

	static SxyEngine& Instance();

	//
	// ö�����н���, �ᷢ���ź�: sgEnumProcess, sgEnumProcessDone 
	// sgListProcess: ÿ����һ������ʱ����
	// sgListProcessDone: ����ѭ����������
	//
	void EnumProcess(const QString& filter = QString());
	//
	// ��ӽ��̵�_ProcessList����
	//
	void AppendProcess(SProcess* pProcess);
	void SelectProcess(SProcess* pProcess);
	void RemoveAllProcess();
	LST_PROCESS& GetProcessList();
	bool AttachSelectedProcess();
	SProcess* GetSelectedProcess();

	// TODO��ö�ٴ���

	//
	// ������ֵ
	//
	void Search(EFIND_TYPE type, 
		EFIND_METHOD compare, 
		const QString& valueA, 
		const QString& valueB = QString());

	//
	// ���������������¿�ʼ�µ�����
	//
	void Restart();

	//
	// ��ѯ��ַ�Ƿ���ģ��ľ�̬��ַ
	// [OUT] qModuleName ����ģ�����֣����Ϊ�գ�����˽���ڴ�ҳ
	// [IN]  address ����Ҫ��ѯ�ĵ�ַ
	// [RETURN] �����˽���ڴ�ҳ����-1, ���򷵻�ģ�龵���ƫ��
	// 
	qint64 QueryModuleOffset(QString& qModuleName, quint64 address);

signals:
	void sgEnumProcess(qint32, SProcess*);
	void sgEnumProcessDone();

private:
	SxyEngine();

	LST_PROCESS _ProcessList;
	SProcess* _AttachProcess;
};



#define SEngine SxyEngine::Instance()
