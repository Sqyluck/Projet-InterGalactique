using Constellation;
using Constellation.Package;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.ComponentModel;

namespace brain2._0
{
    public class Program : PackageBase
    {
        private List<User> Users;
        private List<Notif> Notifs;

        [StateObjectLink("ESP8266_Package", "distance")]
        public StateObjectNotifier Distance { get; set; }

        [StateObjectLink("ESP8266_Package", "badge")]
        public StateObjectNotifier UID { get; set; }

        private int nextTime = -1;

        Boolean DoorOpened = false;



        static void Main(string[] args)
        {
            PackageHost.Start<Program>(args);
        }

        public override void OnStart()
        {
            PackageHost.WriteInfo("Package starting - IsRunning: {0} - IsConnected: {1}", PackageHost.IsRunning, PackageHost.IsConnected);
            Users = new List<User>();
            Notifs = new List<Notif>();
            this.Distance.ValueChanged += Distance_ValueChanged;
            this.UID.ValueChanged += UID_ValueChanged;
            Users.Add(new User("ad795bb", false, "monsieur", "facteur"));
            Users.Add(new User("4a96d8b", true, "pierre", "hourde"));
            PackageHost.PushStateObject("Users",Users);
            while (PackageHost.IsRunning)
            {

            }
        }

        private void Distance_ValueChanged(object sender, StateObjectChangedEventArgs e)
        {
            if (DoorOpened)
            {
                if (!e.IsNew)
                {
                    if (e.OldState.IsExpired)
                    {
                        PackageHost.WriteInfo("du courrier est arrivé");
                        Notifs.Add(new Notif("post", DateTime.Now));
                        pushOnConstellation();
                    }
                }
                else
                {
                    PackageHost.WriteInfo("du courrier est arrivé");
                    Notifs.Add(new Notif("post", DateTime.Now));
                    pushOnConstellation();
                }
            }
        }

       
        public void pushOnConstellation()
        {
            PackageHost.PushStateObject("Notification", Notifs);
        }

        public void changeDoor()
        {
            if (DoorOpened)
            {
                DoorOpened = false;
            }
            else
            {
                DoorOpened = true;
            }
            PackageHost.PushStateObject("doorOpened", DoorOpened);
        }

        public void UID_ValueChanged(object sender, StateObjectChangedEventArgs e){

            PackageHost.WriteInfo(e.NewState.DynamicValue);
            if (!e.IsNew) {
                int id = findUser(e.NewState.DynamicValue);
                if (id != -1) {
                    changeDoor();
                    if (DoorOpened) {
                        if (this.Users[id].client) {
                            Notifs = new List<Notif>();
                            pushOnConstellation();
                        }
                        else{
                            Notifs.Add(new Notif("colis", DateTime.Now));
                            pushOnConstellation();
                        }
                    }
                }
                else {
                    PackageHost.PurgeStateObjects("newUser", e.NewState.DynamicValue);
                }
            }

        }

        [MessageCallback]
        public void AddUser(string uid, Boolean type, string firstName, string name)
        {
            Users.Add(new User(uid, type, firstName, name));
            PackageHost.PushStateObject("Users", Users);
        }

        public int findUser(string uid)
        {
            for(int i = 0; i < this.Users.Count; i++)
            {
                if (uid == Users[i].uid)
                {
                    return i;
                }
            }
            return -1;
        }
    }
}
