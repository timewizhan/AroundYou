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
    @IBOutlet weak var downloadindicator: UIActivityIndicatorView!
    
    var dataStatus : E_DATA_CONNECTION_TYPE = .E_DATA_READY
    var refreshStatus : E_REFRESH_TYPE = .E_REFRESH_READY
    
    let textCellIdentifier = "TextCell"
    
    var menuArray : [String] = []
    var arrayMenuData : [StoresMenuData] = []
    var storeData = StoresData()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        tableView.delegate = self
        tableView.dataSource = self
    }
    
    override func viewDidAppear(animated: Bool) {
        if refreshStatus == .E_REFRESH_DONE {
            return
        }
        
        // start Indicator until receiving data from server
        downloadindicator.startAnimating()
        
        // Thread for receiving store data
        let queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
        dispatch_async(queue, { () -> () in
            let nRet : Int
            self.dataStatus = .E_DATA_START
            nRet = self.getMenuDataFromServer()
            if nRet != E_RET_TYPE.E_RET_SUCCESS.rawValue {
                debugPrint("Fail to load stores")
                self.dataStatus = .E_DATA_CANCELL
            }
            else {
                self.dataStatus = .E_DATA_FINISH
            }
        })
        
        while true
        {
            if self.dataStatus != .E_DATA_FINISH {
                continue
            }
            
            debugPrint("Success to download data (store data)")
            downloadindicator.stopAnimating()
            break
        }
        self.tableView.reloadData()
        refreshStatus = .E_REFRESH_DONE
    }
    
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
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
    }
    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        if let menuDetailViewTableViewController = segue.destinationViewController as? MenuDetailViewTableViewController {
            if let cell = sender as? UITableViewCell {
                let indexPath = tableView.indexPathForCell(cell)
                if let index = indexPath?.row {
                    debugPrint("click button [\(menuArray[index])]")
                    menuDetailViewTableViewController.storeMenuDetailData.storeData = storeData
                    menuDetailViewTableViewController.storeMenuDetailData.storeMenuData = arrayMenuData[index]
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
        stReqData["storeindex"] = storeData.strIndex!
        
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
        //strRecvMsg = " {\"count\" : \"2\", \"data\" : [{\"store\" : \"a\"},{\"store\" : \"b\"}]}"
        var countArray : Int = 0
        if let data = strRecvMsg.dataUsingEncoding(NSUTF8StringEncoding) {
            let json = JSON(data: data)
            countArray = Int(json["count"].stringValue)!
            debugPrint("store array [\(countArray)]")
            
            for item in json["data"].arrayValue {
                let itemMenu = item["menuname"].stringValue
                let itemReputation = item["reputation"].stringValue
                let itemPrice = item["price"].stringValue
                let itemIndex = item["index"].stringValue
                
                debugPrint("Menu Name : [\(itemMenu)]")
                debugPrint("Menu Reputation : [\(itemReputation)]")
                debugPrint("Menu Price : [\(itemPrice)]")
                debugPrint("Menu Index : [\(itemIndex)]")
                menuArray.append(itemMenu)
                
                var savedStoreMenu = StoresMenuData()
                savedStoreMenu.strMenuName = itemMenu
                savedStoreMenu.strReputation = itemReputation
                savedStoreMenu.strPrice = itemPrice
                savedStoreMenu.strIndex = itemIndex
                arrayMenuData.append(savedStoreMenu)
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