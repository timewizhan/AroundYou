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
    var reputationStore : String = ""
    
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
        
        /*
            before loading store view, app have to get data about store menu
            so, users should wait for just seconds
        */
        let nRet : Int
        nRet = getStoreDataFromServer()
        if nRet != E_RET_TYPE.E_RET_SUCCESS.rawValue {
            debugPrint("Fail to load stores")  
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
        if let menuViewTableViewController = segue.destinationViewController as? MenuViewTableViewController {
            if let cell = sender as? UITableViewCell {
                let indexPath = tableViewStores.indexPathForCell(cell)
                if let index = indexPath?.row {
                    debugPrint("click button [\(arrayStores[index])]")
                    menuViewTableViewController.textStoreName = arrayStores[index]
                    menuViewTableViewController.textStoreStar = reputationStore
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
                "reputation" : "3.0"
                "data" : [
                            {"store" : "a"},
                            {"store" : "b"}
                         ]
            }
        */
        strRecvMsg = " {\"count\" : \"2\", \"data\" : [{\"store\" : \"a\"},{\"store\" : \"b\"}]}"
        var countArray : Int = 0
        if let data = strRecvMsg.dataUsingEncoding(NSUTF8StringEncoding) {
            let json = JSON(data: data)
            countArray = Int(json["count"].stringValue)!
            debugPrint("store array [\(countArray)]")
            reputationStore = json["reputation"].stringValue
            for item in json["data"].arrayValue {
                let itemData = item["store"].stringValue
                debugPrint("store name [\(itemData)]")
                arrayStores.append(itemData)
            }
        }
        
        if countArray < 1 {
            let itemNothing : String = "nothing"
            arrayStores.append(itemNothing)
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        else {
            debugPrint("Success to get store data from server")
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
