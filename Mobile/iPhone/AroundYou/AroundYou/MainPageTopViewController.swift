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
    @IBOutlet weak var containerView: UIView!
    
    var customController : MainCustomPageViewController?
    
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

    @IBAction func returnToSelectPage(sender: AnyObject) {
        debugPrint("clicked select page")
        self.customController!.segueIdentifierReceivedFromParent(E_BUTTON_TYPE.E_BUTTON_SELECT_PAGE)
    }
    @IBAction func returnToListPage(sender: AnyObject) {
        debugPrint("clicked list page")
        self.customController!.segueIdentifierReceivedFromParent(E_BUTTON_TYPE.E_BUTTON_LIST_PAGE)
    }
    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        
        if segue.identifier == "EmbededMainCustomSegue" {
            self.customController = segue.destinationViewController as? MainCustomPageViewController
        }
    }
    

}
