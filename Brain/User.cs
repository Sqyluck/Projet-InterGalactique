using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace brain2._0
{
    class User
    {
        public string uid;
        public Boolean client;
        public string name;
        public string firstName;
        public User(string uid, Boolean client, string firstName, string name) {
            this.uid = uid;
            this.client = client;
            this.firstName = firstName;
            this.name = name;
        }
    }
}
