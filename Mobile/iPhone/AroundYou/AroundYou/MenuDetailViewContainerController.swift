//
//  MenuDetailViewContainerController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 16..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MenuDetailViewContainerController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    @IBOutlet weak var commentTableView: UITableView!
    
    @IBOutlet weak var downloadindicator: UIActivityIndicatorView!
    var dataStatus : E_DATA_CONNECTION_TYPE = .E_DATA_READY
    var refreshStatus : E_REFRESH_TYPE = .E_REFRESH_READY
    
    let textCellIdentifier = "CommentCell"
    
    var comments :[Comment] = []
    var storeMenuDetailData = StoresMenuDetailData()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        commentTableView.delegate = self
        commentTableView.dataSource = self
    }
    
    override func viewDidAppear(animated: Bool) {
        if refreshStatus == .E_REFRESH_DONE {
            return
        }
        
        // start Indicator until receiving data from server
        downloadindicator.startAnimating()
        //comments.append(Comment(userId: "TestMan", strComment: "Hello Good store", strStar: "1"))
        
        // Thread for receiving store data
        let queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
        dispatch_async(queue, { () -> () in
            let nRet : Int
            self.dataStatus = .E_DATA_START
            nRet = self.getCommentDataFromServer()
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
        self.commentTableView.reloadData()
        refreshStatus = .E_REFRESH_DONE
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
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
    }
    
    @IBAction func cancelComment(segue : UIStoryboardSegue) {
        
    }
    
    @IBAction func saveComment(segue : UIStoryboardSegue) {
        if let commentViewController = segue.sourceViewController as? CommentViewController {
            
            //add the new comment to the comment array
            if let comment = commentViewController.comment {
                comments.append(comment)
                
                //update the tableView
                let indexPath = NSIndexPath(forRow: comments.count-1, inSection: 0)
                commentTableView.insertRowsAtIndexPaths([indexPath], withRowAnimation: .Automatic)
                
                //update data to server
                // Thread for receiving store data
                let queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
                dispatch_async(queue, { () -> () in
                    let nRet : Int
                    
                    var commentData = StoresMenuComment()
                    commentData.strStoreIndex = self.storeMenuDetailData.storeData.strIndex
                    commentData.strMenuIndex = self.storeMenuDetailData.storeMenuData.strIndex
                    commentData.strWriter = comment.userId!
                    commentData.strReputation = comment.strStar!
                    commentData.strText = comment.strComment!
                    
                    nRet = self.sendCommentDataToServer(commentData)
                    if nRet == E_RET_TYPE.E_RET_SUCCESS.rawValue {
                        debugPrint("Success to load comment data")
                        return
                    }
                    debugPrint("Fail to load stores")
                })
            }
        }
    }
    
    func getCommentDataFromServer() -> Int {
        debugPrint("Try to get menu data from server")
        
        let cNetworkingCommunication = NetworkingCommunication()
        
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_MENUS.rawValue)
        // location is default
        stReqData["storeindex"] = storeMenuDetailData.storeData.strIndex!
        stReqData["menuindex"] = storeMenuDetailData.storeMenuData.strIndex!
        
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
                        {"commentid" : "a", "commentreputation" : "2", "commenttext" : "hi"},
                        {"commentid" : "a", "commentreputation" : "2", "commenttext" : "hi"}
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
                let itemID = item["writer"].stringValue
                let itemReputation = item["reputation"].stringValue
                let itemComment = item["text"].stringValue
                let itemTime = item["writetime"].stringValue
                
                debugPrint("Comment ID [\(itemID)]")
                debugPrint("Comment Reputation [\(itemReputation)]")
                debugPrint("Comment Text [\(itemComment)]")
                debugPrint("Comment Time [\(itemTime)]")
                let userComment = Comment(userId: itemID, strComment: itemReputation, strStar: itemComment)
                
                debugPrint("Insert comment data")
                comments.append(userComment)
            }
        }
        
        if countArray < 1 {
            debugPrint("There is no comments data")
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        debugPrint("Success to get menu data from server")
        return E_RET_TYPE.E_RET_SUCCESS.rawValue
    }
    
    func sendCommentDataToServer(commentData : StoresMenuComment) -> Int {
        debugPrint("Try to get menu data from server")
        
        let cNetworkingCommunication = NetworkingCommunication()
        
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_MENUS.rawValue)
        stReqData["writer"] = commentData.strWriter!
        stReqData["reputation"] = commentData.strReputation!
        stReqData["text"] = commentData.strText!
        //stReqData["writetime"] = ""
        
        
        let strJsonReqData = stReqData.toJSONString()
        var strRecvMsg : String = ""
        var nRet : Int
        nRet = cNetworkingCommunication.networkingWithServer(strJsonReqData, nMsgType: E_PROTO_REQ_TYPE.E_PROTO_REQ_HEADER_MENUS, strRecvMsg: &strRecvMsg)
        if (nRet > 0) {
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        return E_RET_TYPE.E_RET_SUCCESS.rawValue
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
