//
//  StoresViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 10..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class StoresViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    @IBOutlet weak var tableViewStores: UITableView!
    @IBOutlet weak var menuButton: UIBarButtonItem!
    

    let textCellIdentifier = "cellStores"
    var arrayStores : [String] = ["부산식당", "형제갈비", "미가", "맥도날드", "버거킹"]
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        tableViewStores.delegate = self
        tableViewStores.dataSource = self
        
        if self.revealViewController() != nil {
            menuButton.target = self.revealViewController()
            menuButton.action = "revealToggle:"
            self.view.addGestureRecognizer(self.revealViewController().panGestureRecognizer())
        }
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        print(arrayStores.count)
        return arrayStores.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell
    {
        let cell = tableView.dequeueReusableCellWithIdentifier(textCellIdentifier, forIndexPath: indexPath)
        
        let row = indexPath.row
        cell.textLabel?.text = arrayStores[row]
        
        return cell
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
        let row = indexPath.row
        debugPrint(arrayStores[row])
        //print(menuArray[row])
    }
    
    // MARK: - Navigation
    
    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        if let menuViewController = segue.destinationViewController as? MenuViewController {
            if let cell = sender as? UITableViewCell {
                let indexPath = tableViewStores.indexPathForCell(cell)
                if let index = indexPath?.row {
                    debugPrint("click button [\(arrayStores[index])]")
                    menuViewController.labelStore = arrayStores[index]
                }
            }
        }
    }
    
    
    // Network Function
    func getMenuDataFromServer() {
        debugPrint("Try to get datat from server")
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
