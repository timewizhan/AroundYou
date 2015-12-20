//
//  SelectCustomPageViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 12. 20..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class SelectCustomPageViewController: UIViewController {

    var viewController : UIViewController!
    var lastViewController: UIViewController!
    var segueIdentifier : String!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.segueIdentifierReceivedFromParent(E_BUTTON_TYPE.E_BUTTON_SELECT_DETAIL_STORE_PAGE)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func segueIdentifierReceivedFromParent(eButtonType : E_BUTTON_TYPE){
        if eButtonType == E_BUTTON_TYPE.E_BUTTON_SELECT_DETAIL_STORE_PAGE {
            self.segueIdentifier = "SelectStoreDetailPage"
            self.performSegueWithIdentifier(self.segueIdentifier, sender: nil)
        }
        else if eButtonType == E_BUTTON_TYPE.E_BUTTON_SELECT_DETAIL_MENU_PAGE {
            self.segueIdentifier = "SelectMenuDetailPage"
            self.performSegueWithIdentifier(self.segueIdentifier, sender: nil)
        }
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if segue.identifier == segueIdentifier{
            if lastViewController != nil{
                lastViewController.view.removeFromSuperview()
            }
            
            viewController = segue.destinationViewController
            self.addChildViewController(viewController)
            viewController.view.frame = CGRect(x: 0,y: 0, width: self.view.frame.width,height: self.view.frame.height)
            self.view.addSubview(viewController.view)
            
            viewController.didMoveToParentViewController(self)
            lastViewController = viewController
        }
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
