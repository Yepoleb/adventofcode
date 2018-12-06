#include <fstream>
#include <iomanip>
#include <ctime>
#include <limits>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <map>



enum EventType
{
    EV_BEGIN_SHIFT,
    EV_WAKE_UP,
    EV_FALL_ASLEEP
};

struct Event
{
    std::tm datetime;
    EventType evtype;
    int guard_num;
};


bool operator<(const std::tm& a, const std::tm& b)
{
    if (a.tm_year != b.tm_year) {
        return a.tm_year < b.tm_year;
    } else if (a.tm_mon != b.tm_mon) {
        return a.tm_mon < b.tm_mon;
    } else if (a.tm_mday != b.tm_mday) {
        return a.tm_mday < b.tm_mday;
    } else if (a.tm_hour != b.tm_hour) {
        return a.tm_hour < b.tm_hour;
    } else if (a.tm_min != b.tm_min) {
        return a.tm_min < b.tm_min;
    } else {
        return a.tm_sec < b.tm_sec;
    }
}

int sleepiest_index(const std::array<int,60>& minutes)
{
    int max_minutes = 0;
    int max_index = 0;
    for (int min_i = 0; min_i < 60; min_i++) {
        if (minutes[min_i] > max_minutes) {
            max_index = min_i;
            max_minutes = minutes[min_i];
        }
    }
    return max_index;
}

int main()
{
    std::ifstream file("input.txt");
    assert(file.good());

    std::vector<Event> events;
    while (true) {
        char c = 0;
        file >> c;
        if (c != '[') {
            break;
        }
        std::tm datetime = {};
        file >> std::get_time(&datetime, "%Y-%m-%d %H:%M");
        file.ignore(2);
        file >> c;
        if (c == 'w') {
            events.push_back({datetime, EV_WAKE_UP, 0});
        } else if (c == 'f') {
            events.push_back({datetime, EV_FALL_ASLEEP, 0});
        } else if (c == 'G') {
            file.ignore(6);
            int guard_num;
            file >> guard_num;
            events.push_back({datetime, EV_BEGIN_SHIFT, guard_num});
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::sort(events.begin(), events.end(),
        [](const Event& a, const Event& b) {
            return a.datetime < b.datetime;
        }
    );

    std::map<int,int> time_asleep;
    std::map<int,std::array<int,60>> sleep_minutes;
    int current_guard;
    int fell_asleep;
    for (const Event& ev : events) {
        if (ev.evtype == EV_BEGIN_SHIFT) {
            current_guard = ev.guard_num;
        } else if (ev.evtype == EV_FALL_ASLEEP) {
            fell_asleep = ev.datetime.tm_min;
        } else if (ev.evtype == EV_WAKE_UP) {
            int woke_up = ev.datetime.tm_min;
            time_asleep[current_guard] += woke_up - fell_asleep;
            for (int min_i = fell_asleep; min_i < woke_up; min_i++) {
                sleep_minutes[current_guard][min_i]++;
            }
        }
    }

    int sleepiest_guard = std::max_element(
        time_asleep.begin(), time_asleep.end(),
        [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
            return a.second < b.second;
        }
    )->first;

    int max_index = sleepiest_index(sleep_minutes[sleepiest_guard]);

    std::printf("Sleepiest guard: %d\n", max_index * sleepiest_guard);

    int all_max_guard = 0;
    int all_max_i = 0;
    int all_max_count = 0;
    for (const auto& sleep_pair : sleep_minutes) {
        int guard_num = sleep_pair.first;
        const std::array<int,60>& minutes = sleep_pair.second;
        int guard_max_i = sleepiest_index(minutes);
        int guard_count = minutes[guard_max_i];
        if (guard_count > all_max_count) {
            all_max_guard = guard_num;
            all_max_i = guard_max_i;
            all_max_count = guard_count;
        }
    }

    std::printf("Repeat sleeper: %d\n", all_max_guard * all_max_i);

    return 0;
}
