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
    
    override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?) {
        textNickName.endEditing(true)
        textEMail.endEditing(true)
    }
    
    @IBAction func textNickNameButton(sender: AnyObject) {
        sender.resignFirstResponder()
    }
    @IBAction func textEMailButton(sender: AnyObject) {
        sender.resignFirstResponder()
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
        let cNetworkingCommunication = NetworkingCommunication()
        
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_SIGNIN.rawValue)
        stReqData["nickname"] = strNickName!
        stReqData["email"] = strEmail!
        stReqData["CallID"] = callID
        
        let strJsonReqData = stReqData.toJSONString()
        var strRecvMsg : String = ""
        nRet = cNetworkingCommunication.networkingWithServer(strJsonReqData, nMsgType: E_PROTO_REQ_TYPE.E_PROTO_REQ_HEADER_SIGNIN, strRecvMsg: &strRecvMsg)
        if (nRet > 0) {
            return
        }*/
        
        UserAccount.setUserNickName(strNickName!)
        UserAccount.setUserEmail(strEmail!)
        
        debugPrint("Success to sign in for user account")
        let vc : AnyObject! = self.storyboard?.instantiateViewControllerWithIdentifier("Reveal View")
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
        
        let cNetworkingCommunication = NetworkingCommunication()
        
        var stReqData : BuildJSON = BuildJSON()
        stReqData["request"] = String(E_PROTO_REQ_TYPE.E_PROTO_REQ_DATA_SIGNUP.rawValue)
        stReqData["nickname"] = strNickName!
        stReqData["email"] = strEmail!
        stReqData["CallID"] = callID
        
        let strJsonReqData = stReqData.toJSONString()
        var strRecvMsg : String = ""
        nRet = cNetworkingCommunication.networkingWithServer(strJsonReqData, nMsgType: E_PROTO_REQ_TYPE.E_PROTO_REQ_HEADER_SIGNUP, strRecvMsg: &strRecvMsg)
        if (nRet > 0) {
            return
        }
        
        UserAccount.setUserNickName(strNickName!)
        UserAccount.setUserEmail(strEmail!)

        debugPrint("Success to sign up for user account")
        let vc : AnyObject! = self.storyboard?.instantiateViewControllerWithIdentifier("Reveal View")
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
