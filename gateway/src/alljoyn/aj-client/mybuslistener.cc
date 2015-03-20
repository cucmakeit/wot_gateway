#include "mybuslistener.h"
#include "clientbusattachment.h"
#include <iostream>
#include <signal.h>

using namespace std;

extern ClientBusAttachment* clientbus;
extern MyBusObject* obj;

extern SessionPort PORT;
extern char dev_prop[1024];

SessionId g_sessionId;

extern volatile sig_atomic_t g_joinComplete;

void MyBusListener::FoundAdvertisedName(const char* name, TransportMask transport, const char* namePrefix)
{
	/* Since we are in a callback we must enable concurrent callbacks before calling a synchronous method. */
	clientbus->EnableConcurrentCallbacks();
	SessionOpts opts(SessionOpts::TRAFFIC_MESSAGES, true, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);

	QStatus status = clientbus->JoinSession(name, PORT, this, g_sessionId, opts);
	if (ER_OK == status) {
		cout << "Joined conversation. Session id=" << g_sessionId << endl;

	} else {
		cout << "JoinSession failed (status=" << QCC_StatusText(status) << endl;
	}

	uint32_t timeout = 40;
	status = clientbus->SetLinkTimeout(g_sessionId, timeout);
	if (ER_OK != status) {
		cout << "Set link timeout failed\n";
	}
	g_joinComplete = true;
	obj->sendDevPropSignal(dev_prop);
}

void MyBusListener::LostAdvertisedName(const char* name, TransportMask transport, const char* namePrefix)
{
	//�ͻ��˵���
	cout << "client is losing advertised name:" << name << endl;
}

void MyBusListener::SessionLost (SessionId sessionId)
{
	// �ͻ��˵��ã��ȵ����������ٵ���LostAdvertisedName
	cout << "\nsession " << sessionId << " lost\n";
}

void MyBusListener::NameOwnerChanged(const char* busName, const char* previousOwner, const char* newOwner)
{
	// ���Ǳ��Զ����ӵ��úü��Σ�Ϊʲô ��
	printf("NameOwnerChanged: name=%s, oldOwner=%s, newOwner=%s\n", busName, previousOwner ? previousOwner : "<none>",
		newOwner ? newOwner : "<none>");
	if (newOwner == NULL) {
		printf("joiner '%s' exited\n",previousOwner);
	}
}

void MyBusListener::BusDisconnected()
{

}
