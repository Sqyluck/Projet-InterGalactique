import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { ToastController } from 'ionic-angular';

@Component({
    selector: 'page-AjoutCompte',
    templateUrl: 'AjoutCompte.html'
})
export class AjoutComptePage {

    constructor(private toastCtrl: ToastController) { }

    /*todo = {}
    logForm() {
        console.log(this.todo)
    }*/


    presentToast() {
        
        let toast = this.toastCtrl.create({
            message: 'Le compte a bien été créé',
            duration: 3000,
            position: 'top'
        });

        toast.onDidDismiss(() => {
            console.log('Dismissed toast');
        });

        toast.present();
    }

    /*ionViewDidLoad() {
        console.log('ionViewDidLoad AjoutComptePage');
    }*/

}
