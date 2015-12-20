//
//  SelectPageViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 12. 16..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class SelectPageViewController: UIViewController {

    var customSelectController : SelectCustomPageViewController?
    var requestedPage : E_BUTTON_TYPE?
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.customSelectController!.segueIdentifierReceivedFromParent(requestedPage!)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        if segue.identifier == "EmbededSelectCustomSegue" {
            self.customSelectController = segue.destinationViewController as? SelectCustomPageViewController
        }
    }
}
