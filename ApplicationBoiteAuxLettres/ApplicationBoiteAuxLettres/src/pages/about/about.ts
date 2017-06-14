import { Component } from '@angular/core';

import { NavController, NavParams } from 'ionic-angular';
import { AjoutComptePage } from '../AjoutCompte/AjoutCompte';

@Component({
  selector: 'page-about',
  templateUrl: 'about.html'
})
export class AboutPage {
      constructor(public navCtrl: NavController) {

    }

  goToOtherPage() {
      this.navCtrl.push(AjoutComptePage);
  }

}
