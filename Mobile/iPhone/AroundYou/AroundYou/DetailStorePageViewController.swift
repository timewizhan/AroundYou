//
//  DetailStorePageViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 12. 24..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class DetailStorePageViewController: UIViewController {

    var viewController : UIViewController!
    var lastViewController: UIViewController!
    var segueIdentifier : String!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    @IBAction func clickUserComment(sender: AnyObject) {
        debugPrint("click User Comment")
        self.segueIdentifier = "UserCommentPage"
        self.performSegueWithIdentifier(self.segueIdentifier, sender: nil)

    }
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if segue.identifier == segueIdentifier {
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

}
