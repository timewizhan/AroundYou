//
//  MenuViewController.swift
//  TestSample
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MenuContainerViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    @IBOutlet weak var tableView: UITableView!
    
    
    var menuArray = ["김치 찌개", "제육 볶음", "김치 찜", "오징어 볶음"]
    let textCellIdentifier = "TextCell"
    var labelStore : String = ""
    var labelChoice : String?
    //var external_row: Int?
    
    var reputationMenu : String = ""
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        tableView.delegate = self
        tableView.dataSource = self
        /*
        let nRet : Int
        nRet = getMenuDataFromServer()
        if nRet != E_RET_TYPE.E_RET_SUCCESS.rawValue {
            debugPrint("Fail to load stores")
        }*/
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
        //external_row = row
        
        labelChoice = menuArray[row]
    }
    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        /*if segue.identifier == "menuContainerSegue" {
            if let menuContainerTableViewController = segue.destinationViewController as? MenuContainerTableViewController {
                menuContainerTableViewController.textStoreName = labelStore
                menuContainerTableViewController.textStoreStar = ""
            }
        }*/
        /*
        if segue.identifier == "menuToDetailSegue" {
            if let menuDetailViewTableViewController = segue.destinationViewController as? MenuDetailViewTableViewController {
                if let cell = sender as? UITableViewCell {
                    let indexPath = tableView.indexPathForCell(cell)
                    if let index = indexPath?.row {
                        print(menuArray[index])
                        menuDetailViewTableViewController.textMenuName = menuArray[index]
                    }
                }
            }
        }*/
        
        if let menuDetailViewTableViewController = segue.destinationViewController as? MenuDetailViewTableViewController {
            if let cell = sender as? UITableViewCell {
                let indexPath = tableView.indexPathForCell(cell)
                if let index = indexPath?.row {
                    debugPrint("click button [\(menuArray[index])]")
                    menuDetailViewTableViewController.textMenuName = menuArray[index]
                    menuDetailViewTableViewController.textStoreName = labelStore
                }
            }
        }
    }
    
    func getMenuDataFromServer() -> Int {
        debugPrint("Try to get menu data from server")
        
        let cNetworkingCommunication = NetworkingCommunication()
        
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_MENUS.rawValue)
        // location is default
        stReqData["store"] = labelStore
        
        let strJsonReqData = stReqData.toJSONString()
        var strRecvMsg : String = ""
        var nRet : Int
        nRet = cNetworkingCommunication.networkingWithServer(strJsonReqData, nMsgType: E_PROTO_REQ_TYPE.E_PROTO_REQ_HEADER_MENUS, strRecvMsg: &strRecvMsg)
        if (nRet > 0) {
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        
        /*
        Parse received data (store data)
        
        a following string is example that server send for response.
        @param : count param is number of array
        
        {
            "count" : "2"
            "data" : [
                        {"menuname" : "a", "menureputation" : "1"},
                        {"menuname" : "b", "menureputation" : "1"}
                     ]
        }
        */
        strRecvMsg = " {\"count\" : \"2\", \"data\" : [{\"store\" : \"a\"},{\"store\" : \"b\"}]}"
        var countArray : Int = 0
        if let data = strRecvMsg.dataUsingEncoding(NSUTF8StringEncoding) {
            let json = JSON(data: data)
            countArray = Int(json["count"].stringValue)!
            debugPrint("store array [\(countArray)]")
            reputationMenu = json["reputation"].stringValue
            for item in json["data"].arrayValue {
                let itemMenu = item["menuname"].stringValue
                let itemReputation = item["menureputation"].stringValue
                
                debugPrint("Menu Name : [\(itemMenu)]")
                debugPrint("Menu Reputation : [\(itemReputation)]")
                menuArray.append(itemMenu)
            }
        }
        
        if countArray < 1 {
            let itemNothing : String = "nothing"
            menuArray.append(itemNothing)
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        else {
            debugPrint("Success to get menu data from server")
            return E_RET_TYPE.E_RET_SUCCESS.rawValue
        }
    }
}