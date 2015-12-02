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
    
    @IBOutlet weak var downloadindicator: UIActivityIndicatorView!
    var dataStatus : E_DATA_CONNECTION_TYPE = .E_DATA_READY
    var refreshStatus : E_REFRESH_TYPE = .E_REFRESH_READY
    
    let textCellIdentifier = "cellStores"
    
    // Array for showing to user
    var arrayStores : [String] = []
    
    // Array for communication
    var arrayStoreData : [StoresData] = []
    
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
    
    override func viewDidAppear(animated: Bool) {
        if refreshStatus == .E_REFRESH_DONE {
            return
        }
        
        // start Indicator until receiving data from server
        downloadindicator.startAnimating()
        /*
        var testvalue = StoresData()
        testvalue.strIndex = "1"
        testvalue.strReputation = "3"
        testvalue.strStoreName = "aaa"
        arrayStores.append(testvalue.strStoreName!)
        arrayStoreData.append(testvalue)
        */
        // Thread for receiving store data
        /*
        let queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
        dispatch_async(queue, { () -> () in
            let nRet : Int
            self.dataStatus = .E_DATA_START
            nRet = self.getStoreDataFromServer()
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
        self.tableViewStores.reloadData()
        refreshStatus = .E_REFRESH_DONE*/
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
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
    }
    
    // MARK: - Navigation
    
    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        if let menuViewTableViewController = segue.destinationViewController as? MenuViewTableViewController {
            if let cell = sender as? UITableViewCell {
                let indexPath = tableViewStores.indexPathForCell(cell)
                if let index = indexPath?.row {
                    debugPrint("click button [\(arrayStores[index])]")
                    menuViewTableViewController.storeData = arrayStoreData[index]
                }
            }
        }
    }
    
    
    // Network Function
    func getStoreDataFromServer() -> Int {
        debugPrint("Try to get store data from server")
        
        let cNetworkingCommunication = NetworkingCommunication()
        
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_STORES.rawValue)
        // location is default
        stReqData["location"] = "sinchon"
        
        let strJsonReqData = stReqData.toJSONString()
        var strRecvMsg : String = ""
        var nRet : Int
        nRet = cNetworkingCommunication.networkingWithServer(strJsonReqData, nMsgType: E_PROTO_REQ_TYPE.E_PROTO_REQ_HEADER_STORES, strRecvMsg: &strRecvMsg)
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
                            {"storename" : "a", "storereputation" : "3"},
                            {"storename" : "b", "storereputation" : "2"}
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
                let itemStore : String = item["storename"].stringValue
                let itemReputation : String = item["reputation"].stringValue
                let itemIndex : String = item["index"].stringValue
                
                debugPrint("Store name : [\(itemStore)]")
                debugPrint("Store reputation : [\(itemReputation)]")
                debugPrint("Store Index : [\(itemIndex)]")
                
                var savedStoreData = StoresData()
                savedStoreData.strStoreName = itemStore
                savedStoreData.strReputation = itemReputation
                savedStoreData.strIndex = itemIndex
                
                arrayStores.append(itemStore)
                arrayStoreData.append(savedStoreData)
            }
        }
        
        if countArray < 1 {
            let itemNothing : String = "nothing"
            arrayStores.append(itemNothing)
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        else {
            return E_RET_TYPE.E_RET_SUCCESS.rawValue
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
