//
//  MenuContainerTableViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 11..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MenuViewTableViewController: UITableViewController {
    @IBOutlet weak var labelStoreName: UILabel!
    @IBOutlet weak var labelStoreStar: UILabel!

    var textStoreName : String?
    var textStoreStar : String?
    override func viewDidLoad() {
        super.viewDidLoad()

        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

        // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
        // self.navigationItem.rightBarButtonItem = self.editButtonItem()
        
        labelStoreName.text = textStoreName
        labelStoreStar.text = textStoreStar
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        
        if segue.identifier == "menucontainer" {
            if let menuContainerViewController = segue.destinationViewController as? MenuContainerViewController {
                menuContainerViewController.labelStore = textStoreName!
            }
        }

    }
    
    
    @IBAction func alarmToMenuView(segue : UIStoryboardSegue) {
        if let menuContainerViewController = segue.sourceViewController as? MenuContainerViewController {
            let strData = menuContainerViewController.labelChoice
            debugPrint(strData)
        }
        
    }

}