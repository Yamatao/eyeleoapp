#include "task_mgr.h"
#include "main.h"
#include "wx/timer.h"

TaskManager *g_TaskMgr = 0;

wxDEFINE_EVENT(EXECUTE_TASK_EVENT, wxCommandEvent);

TaskManager::TaskManager()
    : wxThread(wxTHREAD_DETACHED)
    , _endSignal(0) {
    Create();
}

TaskManager::~TaskManager() {
}

void TaskManager::AddTask(const wxString &address, int duration_ms) {
    assert(!address.empty());

    if (address.empty())
        return;

    if (duration_ms < 0)
        return;

    wxMilliClock_t current_time = ::wxGetLocalTimeMillis();
    wxMilliClock_t execute_time = current_time + duration_ms;

    {
        wxCriticalSectionLocker locker(_cs);

        RemoveTasks(address, false);

        TaskRecord r = {execute_time, duration_ms, address};
        _tasks.push_back(r);
    }
}

void TaskManager::RemoveTasks(const wxString &address, bool lock) {
    if (lock)
        _cs.Enter();

    TaskList::iterator end = _tasks.end();
    TaskList::iterator it = _tasks.begin();
    while (it != end) {
        TaskListItem const &item = (*it);
        if (item.address == address)
            it = _tasks.erase(it);
        else
            ++it;
    }

    if (lock)
        _cs.Leave();
}

void TaskManager::StopTasks() {
    wxCriticalSectionLocker locker(_cs);

    InterlockedIncrement(&_endSignal);
}

wxThread::ExitCode TaskManager::Entry() {
    while (_endSignal == 0 && !TestDestroy()) {
        ::wxMilliSleep(20);

        wxMilliClock_t now = ::wxGetLocalTimeMillis();

        {
            wxCriticalSectionLocker locker(_cs);

            TaskList::iterator end = _tasks.end();
            TaskList::iterator it = _tasks.begin();
            while (it != end) {
                if (TestDestroy() || _endSignal != 0)
                    return 0;

                TaskListItem &item = (*it);

                wxMilliClock_t execute_time = item.execute_time;

                if (now >= execute_time) {
                    const wxString &address = item.address;

                    wxMilliClock_t start_time = item.execute_time - item.duration;

                    /*wxMilliClock_t real_duration = now - start_time;
                    float f = float(real_duration.ToDouble() / item.duration.ToDouble()); // correction coef*/

                    wxCommandEvent *event = new wxCommandEvent(EXECUTE_TASK_EVENT, 0);

                    assert(!address.empty());

                    TaskPayload *pl = new TaskPayload(); // it will be freed in another thread
                    pl->start_time = start_time;
                    pl->task = nullptr;
                    pl->address = address;
                    pl->duration = item.duration;
                    pl->check = 12345;

                    event->SetClientData(pl);

                    wxQueueEvent(getApp(), event);

                    item.execute_time = now + item.duration;
                }

                ++it;
            }
        }
    }
    g_TaskMgr = 0;
    return 0;
}