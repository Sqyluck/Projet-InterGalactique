using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace brain2._0
{
    class Notif
    {
        public string type;
        public int year;
        public int month;
        public int day;
        public int hour;
        public int minute;
        public Notif(string type, DateTime date)
        {
            this.type = type;
            this.year = date.Year;
            this.month = date.Month;
            this.day = date.Day;
            this.hour = date.Hour;
            this.minute = date.Minute;
        }
    }
}
