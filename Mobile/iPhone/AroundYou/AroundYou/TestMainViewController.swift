//
//  TestMainViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 5..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class TestMainViewController: UIViewController {

    @IBOutlet weak var main2Image: UIImageView!
    @IBOutlet weak var main3Image: UIImageView!
    override func viewDidLoad() {
        super.viewDidLoad()
        
        main2Image.image = UIImage(named: "main2")
        main3Image.image = UIImage(named: "main3")

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
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
