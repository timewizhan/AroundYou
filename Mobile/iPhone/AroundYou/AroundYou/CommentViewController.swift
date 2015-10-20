//
//  CommnetViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 5..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class CommentViewController: UITableViewController {
    @IBOutlet weak var textComment: UITextField!
    @IBOutlet weak var selectStar: UILabel!
    
    var comment : Comment?
    var star:String = "1" {
        didSet {
            selectStar.text? = star
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func backToComment(segue : UIStoryboardSegue) {
        if let starViewController = segue.sourceViewController as? StarViewController,
            selectedStar = starViewController.selectedStar {
                    star = selectedStar
        }
    }
    
    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if segue.identifier == "saveComment" {
            comment = Comment(userId: "test", strComment: textComment.text, strStar: selectStar.text)
            print(comment)
        }
        if segue.identifier == "selectStar" {
            if let starViewController = segue.destinationViewController as? StarViewController {
                starViewController.selectedStar = star
            }
        }
    }
    
}
