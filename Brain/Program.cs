using Constellation;
using Constellation.Package;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.ComponentModel;
using System.IO;
using Newtonsoft.Json;

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

        Boolean ActivateMotor = false;



        static void Main(string[] args)
        {
            PackageHost.Start<Program>(args);
        }

        public override void OnStart()
        {
            PackageHost.WriteInfo("Package starting - IsRunning: {0} - IsConnected: {1}", PackageHost.IsRunning, PackageHost.IsConnected);
            Users = new List<User>();
            Notifs = new List<Notif>();
            this.loadUsers();
            this.Distance.ValueChanged += Distance_ValueChanged;
            this.UID.ValueChanged += UID_ValueChanged;
            initializeStateObject();

            while (PackageHost.IsRunning)
            {

            }
        }

        public override void OnPreShutdown()
        {
            saveUsers();
        }

        public void initializeStateObject()
        {
            PackageHost.PushStateObject("Users", Users);
            PackageHost.PushStateObject("Notification", Notifs);
            PackageHost.PushStateObject("ActivateMotor", ActivateMotor);
            PackageHost.PushStateObject("newUser", "");
        }

        public void loadUsers()
        {
            string line;
            StreamReader file = new StreamReader(@"C:\Luc\cours\CIR 3\Constellation\brain\brain\UsersSave.txt");
            while((line = file.ReadLine())!= null)
            {
                string uid = line;
                string client = file.ReadLine();
                Boolean b;
                string firstName = file.ReadLine();
                string name = file.ReadLine();
                if(client == "True")
                {
                    b = true;
                }
                else
                {
                    b = false;
                }
                Users.Add(new User(uid, b , firstName, name));
            }
        }
        public void saveUsers() {
            List<string> save = new List<string>();
            StreamWriter file = new StreamWriter(@"C:\Luc\cours\CIR 3\Constellation\brain\brain\UsersSave.txt");

            foreach (User user in Users)
            {
                file.WriteLine(user.uid);
                file.WriteLine(user.client.ToString());
                file.WriteLine(user.firstName);
                file.WriteLine(user.name);
            }

            file.Close();
        }
        private void Distance_ValueChanged(object sender, StateObjectChangedEventArgs e)
        {
            if (!ActivateMotor)
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

        public void changeMotor()
        {
            if (ActivateMotor)
            {
                ActivateMotor = false;
                PackageHost.WriteInfo("porte fermée");
            }
            else
            {
                ActivateMotor = true;
                PackageHost.WriteInfo("porte ouverte");
            }
            PackageHost.PushStateObject("ActivateMotor", ActivateMotor);
        }

        public void UID_ValueChanged(object sender, StateObjectChangedEventArgs e){

            PackageHost.WriteInfo(e.NewState.DynamicValue);
            int id = findUser(e.NewState.DynamicValue);
            if (id != -1) {
                changeMotor();
                if (ActivateMotor) {
                    if (this.Users[id].client) {
                        Notifs = new List<Notif>();
                        PackageHost.WriteInfo($"{this.Users[id].firstName} {this.Users[id].name} a récupéré le courrier");
                        pushOnConstellation();
                     }
                     else{
                        Notifs.Add(new Notif("colis", DateTime.Now));
                        PackageHost.WriteInfo("un colis est arrivé");
                        pushOnConstellation();
                     }
                 }
             }
            else {
                PackageHost.PushStateObject("newUser", e.NewState.DynamicValue);
                PackageHost.WriteInfo($"badge : {e.NewState.DynamicValue} non connu");
            }
            

        }

        [MessageCallback]
        public void AddUser(string uid, Boolean type, string firstName, string name)
        {
                Users.Add(new User(uid, type, firstName, name));
                PackageHost.WriteInfo("ajout carte validé");
                PackageHost.PushStateObject("Users", Users);
            saveUsers();
        }

        [MessageCallback]
        public void DeleteUser(string uid, Boolean type, string firstName, string name)
        {
            int id = FindUser(uid, type, firstName, name);
            if(id!=-1){
                Users.Remove(Users[id]);
                PackageHost.WriteInfo($"le compte de {firstName} {name} a été supprimé");

                PackageHost.PushStateObject("Users", Users);
                saveUsers();
            }
        }

        public int FindUser(string uid, Boolean type, string firstName, string name) {
            for (int i = 0; i < this.Users.Count; i++)
            {
                if ((Users[i].name == name) && (Users[i].uid == uid) && (Users[i].client == type) && (Users[i].firstName == firstName))
                {
                    return i;
                }
            }
            return -1;
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
