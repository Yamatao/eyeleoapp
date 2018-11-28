#pragma once
#include <list>
#include <utility>
#include "wx/thread.h"
#include "wx/stopwatch.h"
#include "wx/event.h"

wxDECLARE_EVENT(EXECUTE_TASK_EVENT, wxCommandEvent);

class ITask
{
public:
	virtual ~ITask() {}
	virtual void ExecuteTask(float f, long time_went) = 0; // 1st - correction coef, 2nd - time went
};

typedef ITask * TaskPtr;

struct TaskRecord
{
	wxMilliClock_t execute_time;
	wxMilliClock_t duration;
	wxString address;
};

struct TaskPayload // this data is transfered to (in another thread) the executor
{
	wxMilliClock_t start_time;
	wxMilliClock_t duration;
	TaskPtr task;
	wxString address;
	int check;
};

class TaskManager : public wxThread
{
public:
	TaskManager();
	virtual ~TaskManager();
	
	void AddTask(const wxString &address, int ms);
	void RemoveTasks(const wxString &address, bool lock = true);
	void StopTasks();

private:
	typedef TaskRecord TaskListItem;
	typedef std::list<TaskListItem> TaskList;
	TaskList _tasks;
	
	volatile long _endSignal;

	wxCriticalSection _cs;

protected:
	virtual wxThread::ExitCode Entry();
};

extern TaskManager * g_TaskMgr;