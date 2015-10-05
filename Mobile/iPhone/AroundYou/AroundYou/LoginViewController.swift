//
//  LoginViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit
import CoreTelephony

class LoginViewController: UIViewController {

    @IBOutlet weak var textNickName: UITextField!
    @IBOutlet weak var textEMail: UITextField!
    @IBOutlet weak var textHiddenMsg: UILabel!
    
    var callID : String = ""
    var networking : CNetworking = CNetworking()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        getCallID()
        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBAction func pressSignIn(sender: AnyObject) {
        let strNickName = textNickName.text
        var nRet = checkNickName(strNickName)
        if nRet != E_RET_TYPE.E_RET_SUCCESS.rawValue {
            textHiddenMsg.text = "Invalid NickName"
            textHiddenMsg.hidden = false
            return
        }
        
        let strEmail = textEMail.text
        nRet = checkEMail(strEmail)
        if nRet != E_RET_TYPE.E_RET_SUCCESS.rawValue {
            textHiddenMsg.text = "Invalid EMail"
            textHiddenMsg.hidden = false
            return
        }
        /*
        // Connet To Server
        let networkRet = networking.connectServer()
        
        // Send Header To Server
        var stReqHeader  = BuildJSON()
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_SIGNIN.rawValue)
        stReqData["nickname"] = strNickName!
        stReqData["email"] = strEmail!
        //stReqData["CallID"] = callID
        stReqData["callid"] = "0101112222"
        
        let strJsonReqData = stReqData.toJSONString()
        let sizeToSendData = strJsonReqData.characters.count
        
        stReqHeader["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_HEADER_SIGNIN.rawValue)
        stReqHeader["size"] = String(sizeToSendData)
        
        let strJsonReq = stReqHeader.toJSONString()
        let sizeToSend = strJsonReq.characters.count
        let sizeSent = networking.sendData(strJsonReq)
        if (sizeSent != sizeToSend) {
            debugPrint("Fail to send Msg")
            return
        }
        debugPrint("Succeed to send Header")
        
        // Receive Header Response From Server
        let defaultReceivedSize = 32
        var strReceivedData = networking.recvData(defaultReceivedSize)
        if (strReceivedData.characters.count < 1) {
            debugPrint("Fail to receive Msg")
            return
        }
        debugPrint("Succeed to receieve Msg")
        */
        /*
            For swiftyjson, you have to change string to dictionary.
            and then, call JSON.
            finally, extract value.
        *//*
        var jsonMethod = JSONMethods()
        var dicData = jsonMethod.convertStringToDictionary(strReceivedData)
        let resHeaderJson = JSON(dicData!)
        var nResponseRet = (resHeaderJson.dictionary!["response"]! as JSON).int!
        if (nResponseRet == E_PROTO_RES_TYPE.E_PROTO_RES_SUCCESS.rawValue) {
            debugPrint("Valid response")
        }
        else {
            debugPrint("Invalid response from server")
            return
        }
        
        // Send Data To Server
        let sizeSentData = networking.sendData(strJsonReqData)
        if (sizeSentData != sizeToSendData) {
            debugPrint("Fail to send Msg")
            return
        }
        debugPrint("Succeed to send Header")
        
        // Receive Data From Server
        strReceivedData = networking.recvData(defaultReceivedSize)
        if (strReceivedData.characters.count < 1) {
            debugPrint("Fail to receive Msg")
            return
        }
        debugPrint("Succeed to receieve Msg")
        
        jsonMethod = JSONMethods()
        dicData = jsonMethod.convertStringToDictionary(strReceivedData)
        let resDataJson = JSON(dicData!)
        nResponseRet = (resDataJson.dictionary!["response"]! as JSON).int!
        
        if (nResponseRet == E_PROTO_RES_TYPE.E_PROTO_RES_SUCCESS.rawValue) {
            debugPrint("Valid response")
        }
        else if (nResponseRet == E_PROTO_RES_TYPE.E_PROTO_RES_DATA_NOT_FOUND_EMAIL.rawValue) {
            debugPrint("Valid response")
        }
        else if (nResponseRet == E_PROTO_RES_TYPE.E_PROTO_RES_DATA_NOT_FOUND_NICKNAME.rawValue) {
            debugPrint("Valid response")
        }
        else {
            debugPrint("Invalid response from server")
            return
        }*/
        
        let vc : AnyObject! = self.storyboard?.instantiateViewControllerWithIdentifier("mainnavi")
        self.showViewController(vc as! UIViewController, sender: sender)
    }
    @IBAction func pressSignUp(sender: AnyObject) {
        let strNickName = textNickName.text
        var nRet = checkNickName(strNickName)
        if nRet != E_RET_TYPE.E_RET_SUCCESS.rawValue {
            textHiddenMsg.text = "Invalid NickName"
            textHiddenMsg.hidden = false
            return
        }
        
        let strEmail = textEMail.text
        nRet = checkEMail(strEmail)
        if nRet != E_RET_TYPE.E_RET_SUCCESS.rawValue {
            textHiddenMsg.text = "Invalid EMail"
            textHiddenMsg.hidden = false
            return
        }
        /*
        let cNetworkingCommunication = NetworkingCommunication()
        
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_SIGNUP.rawValue)
        //stReqData["nickname"] = strNickName!
        //stReqData["email"] = strEmail!
        //stReqData["CallID"] = callID
        stReqData["nickname"] = "cclab"
        stReqData["email"] = "cclab@gmail.com"
        stReqData["callid"] = "0101112222"
        
        let strJsonReqData = stReqData.toJSONString()
        var strRecvMsg : String = ""
        nRet = cNetworkingCommunication.networkingWithServer(strJsonReqData, nMsgType: E_PROTO_REQ_TYPE.E_PROTO_REQ_HEADER_SIGNUP, strRecvMsg: &strRecvMsg)
        if (nRet > 0) {
            return
        }*/

        debugPrint("Success to sign up for user account")
        let vc : AnyObject! = self.storyboard?.instantiateViewControllerWithIdentifier("mainnavi")
        self.showViewController(vc as! UIViewController, sender: sender)
    }
    
    func checkNickName(strNickName : String?) -> Int
    {
        if (strNickName?.characters.count == 0) || (strNickName == " ") || (strNickName == nil) {
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        return E_RET_TYPE.E_RET_SUCCESS.rawValue
    }
    
    /*
        no use character [~ ! # $ % ^ & * ]
    */
    
    var noUseChar = ["~", "!", "#", "$", "%", "^", "&", "*"]
    func checkEMail(strEMail : String?) -> Int
    {
        if (strEMail!.characters.count == 0) || (strEMail == " ") || (strEMail == nil) {
            return E_RET_TYPE.E_RET_FAIL.rawValue
        }
        
        return E_RET_TYPE.E_RET_SUCCESS.rawValue
    }
    
    func getCallID()
    {
        let cCTCall = CTCall()
        callID = cCTCall.callID
        if (callID.characters.count == 0) {
            callID = ""
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
