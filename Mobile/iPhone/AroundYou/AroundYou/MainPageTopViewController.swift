//
//  MainPageTopViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 12. 2..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MainPageTopViewController: UIViewController {

    @IBOutlet weak var settingButton: UIImageView!
    
    var currentPage : Int = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        if self.revealViewController() != nil {
            //self.view.addGestureRecognizer(self.revealViewController().panGestureRecognizer()
            let tapGesture = UITapGestureRecognizer(target: self.revealViewController(), action: "revealToggle:")

            settingButton.addGestureRecognizer(tapGesture)
            settingButton.userInteractionEnabled = true
            
        }
    }
    
    func tapGesture(gesture: UIGestureRecognizer) {
        print("touch")
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func returnToHome(segue: UIStoryboardSegue) {
        
    }

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
