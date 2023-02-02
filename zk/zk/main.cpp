#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <deque>

using namespace std;

constexpr const char* ERROR_WRONG_NUM_ARGS = "Wrong number of arguments";
constexpr const char* ERROR_WRONG_CONFIG = "Wrong configuration file";
constexpr const char* ERROR_WRONG_N = "Wrong parameter N";
constexpr const char* ERROR_WRONG_K = "Wrong parameter K";
constexpr const char* ERROR_WRONG_INPUT = "Wrong parameter input file";
constexpr const char* ERROR_WRONG_OUTPUT = "Wrong parameter output file";
constexpr const char* ERROR_DATA_FILE = "Wrong data file";


class DataPoint {
public:
    DataPoint(map<string, string>& input, int high, int low, int ER) : date(input["date"]), time(input["time"]), high_(high), low_(low), ER_(ER) {};
    int get_ER()
    {
        return ER_;
    }

    string date;
    string time;
    int high_;
    int low_;
    int ER_;
};
class Open {
public:
    Open(const string& date, const string& time, int ERmean, int ERopen) : date_(date), time_(time), ERmean_(ERmean), ERopen_(ERopen) {};
    string date_;
    string time_;
    int ERmean_;
    int ERopen_;
};

class Forex {
public:
    Forex() {};
    bool handle_arg(const vector<string>& arg)
    {
        if (arg.size() == 1) {
            cout << ERROR_WRONG_NUM_ARGS << endl;
            return false;
        }

        config = arg[1];
        if (arg.size() > 2) {
            output = arg[2];
        }

        fstream f;
        string line;
        f.open(config);
        int count = 0;
        while (!f.eof())
        {
            getline(f, line);
            if (count == 0) {
                movingwin = stoi(line);
                next_index = movingwin;
                count++;
            }
            else if (count == 1) {
                radius = stoi(line);
                count++;
            }
            else {
                data_file = line;
            }
        }
        f.close();

        return true;
    }
    int transform_price(const string& s)
    {
        int num = 0;
        int end = !(7 < s.size()) ? s.size() : 7;

        int i = 0;
        for (; i < end; i++)
        {
            if (s[i] != '.')
            {
                num *= 10;
                num += s[i] - '0';
            }
            else {
                break;
            }
        }

        i++;

        if (i < end) 
        {
            for (int j = i; j != s.size() && j < i + 3; j++)
            {
                num *= 10;
                num += s[j] - '0';
            }
        }

        while (num < 100000) {
            num *= 10;
        }   
        return num;
    }
    void fill_vals()
    {
        vector<string> columns{ "date", "time", "open", "high", "low", "close", "volume" };

        fstream f;
        f.open(data_file);
        while (!f.eof())
        {
            string s;
            map<string, string> m;
            for (int i = 0; i < 7; i++) {
                f >> s;
                m.insert(pair<string, string>(columns[i], s));
            }
            int high = transform_price(m["high"]);
            int low = transform_price(m["low"]);
            DataPoint datap(m, high, low, (high + low) / 2);
            dp.push_back(datap);
        }
        f.close();
    }
    void calc_ermean()
    {
        int sum = 0;
        for (int i = 0; i < movingwin; i++)
        {
            sum += dp[i].get_ER();
        }
        ERmean = sum / movingwin;
    }
    void write_open()
    {
        opened_ = false;
        if (output == "")
        {
            Open o = open.back();
            cout << "O " << o.date_ << " " << o.time_ << " " << o.ERmean_  << " " << o.ERopen_  << endl;
        }
        else
        {
            Open o = open.back();
            ofstream f(output);
            f << "O " << o.date_ << " " << o.time_ << " " << o.ERmean_  << " " << o.ERopen_  << endl;
            f.close();
        }
    }
    bool checkClosing(int ERmean, int ERopen, int Ernew) const
    {
        int size = abs(ERopen - ERmean);
        if (abs(ERopen - Ernew)> size)
        {
            return true;
        }
        return false;
    }
    void move_forward()
    {
        int ERlast = dp[0].get_ER();

        int ERnew = dp[next_index].get_ER();
        if (!(ERmean - radius < ERnew && ERnew < ERmean + radius))
        {
            Open op(dp[next_index].date, dp[next_index].time, ERmean, ERnew);
            open.push_back(op);
            opened = true;
        }

        ERmean = ((ERmean * movingwin) - ERlast + ERnew) / movingwin;
        next_index++;
    }
    bool next()
    {
        return next_index != dp.size();
    }
    bool opened()
    {
        return opened_;
    }
    bool closed()
    {
        return closed_;
    }
private:
    string output { "" };
    string config { "" };
    string data_file { "" };
    int movingwin { 0 };
    int radius { 0 };
    int ERmean { 0 };
    int ERopen { 0 };
    int next_index { 0 };
    
    bool opened_ { false };
    bool closed_ { false };

    deque<Open> open;
    deque<DataPoint> dp;
};

int main(int argc, char** argv)
{
    vector<string> arg(argv, argv + argc);

    Forex fx;
    if (!fx.handle_arg(arg)) 
    {
        return 0;
    }
    fx.fill_vals();
    fx.calc_ermean();


    while (fx.next())
    {
        fx.move_forward();
        if (fx.opened())
        {
            fx.write_open();
        }
        if (fx.closed())
        {
            fx.write_close();
        }
    }

    return 0;
}
