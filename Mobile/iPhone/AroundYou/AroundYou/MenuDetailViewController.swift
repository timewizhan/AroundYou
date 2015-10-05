//
//  MenuDetailViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 4..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MenuDetailViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {

    @IBOutlet weak var commentTableView: UITableView!
    var comments :[Comment] = [Comment(userId: "TestMan", strComment: "Hello Good store", strStar: "1")]
    let textCellIdentifier = "CommentCell"
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        commentTableView.delegate = self
        commentTableView.dataSource = self
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        print(comments.count)
        return comments.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell
    {
        let cell = tableView.dequeueReusableCellWithIdentifier(textCellIdentifier, forIndexPath: indexPath) as! CommentCell
        
        let comment = comments[indexPath.row] as Comment
        cell.comment = comment
        return cell
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
        
        //let row = indexPath.row
        //let Input : String = "Test"
        //swiftBlogs.append(Input)
        //print(swiftBlogs[row])
    }
    
    @IBAction func cancelComment(segue : UIStoryboardSegue) {
        
    }
    
    @IBAction func saveComment(segue : UIStoryboardSegue) {
        if let commentViewController = segue.sourceViewController as? CommnetViewController {
            
            //add the new player to the players array
            if let comment = commentViewController.comment {
                comments.append(comment)
                
                //update the tableView
                let indexPath = NSIndexPath(forRow: comments.count-1, inSection: 0)
                commentTableView.insertRowsAtIndexPaths([indexPath], withRowAnimation: .Automatic)
                
            }
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
