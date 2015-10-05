//
//  StoreViewController.swift
//  TestSample
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class StoreViewController: UIViewController {

    @IBOutlet weak var CenterView: UIWebView!
    @IBOutlet weak var LeftView: UIWebView!
    @IBOutlet weak var RightView: UIWebView!
    
    var dataObject:AnyObject?
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        
        CenterView.loadHTMLString(dataObject as! String, baseURL: NSURL(string: ""))
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