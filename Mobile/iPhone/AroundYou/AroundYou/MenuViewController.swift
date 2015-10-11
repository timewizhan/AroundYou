//
//  MenuViewController.swift
//  TestSample
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MenuViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    @IBOutlet weak var tableView: UITableView!
    
    var menuArray = ["김치 찌개", "제육 볶음", "김치 찜", "오징어 볶음"]
    let textCellIdentifier = "TextCell"
    var labelStore : String?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        tableView.delegate = self
        tableView.dataSource = self
        
    }
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        print(menuArray.count)
        return menuArray.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell
    {
        let cell = tableView.dequeueReusableCellWithIdentifier(textCellIdentifier, forIndexPath: indexPath)
        
        let row = indexPath.row
        cell.textLabel?.text = menuArray[row]
        
        return cell
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
        let row = indexPath.row
        debugPrint(menuArray[row])
        
        //print(menuArray[row])
    }
    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        if segue.identifier == "menuContainerSegue" {
            if let menuContainerTableViewController = segue.destinationViewController as? MenuContainerTableViewController {
                menuContainerTableViewController.textStoreName = labelStore
                menuContainerTableViewController.textStoreStar = ""
            }
        }
        if segue.identifier == "menuToDetailSegue" {
            if let menuDetailViewController = segue.destinationViewController as? MenuDetailViewController {
                if let cell = sender as? UITableViewCell {
                    let indexPath = tableView.indexPathForCell(cell)
                    if let index = indexPath?.row {
                        print(menuArray[index])
                        menuDetailViewController.getMenuName = menuArray[index]
                    }
                }
            }
        }
       
    }
}

