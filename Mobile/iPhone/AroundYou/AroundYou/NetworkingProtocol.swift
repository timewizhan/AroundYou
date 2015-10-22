//
//  NetworkingProtocol.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import Foundation

enum E_PROTO_REQ_TYPE : Int
{
    /*
        Sign In
    */
    case E_PROTO_REQ_HEADER_SIGNIN = 0
    case E_PROTO_REQ_DATA_SIGNIN
    
    /*
        Sign Up
    */
    case E_PROTO_REQ_HEADER_SIGNUP
    case E_PROTO_REQ_DATA_SIGNUP
    
    /*
        Stores
    */
    case E_PROTO_REQ_HEADER_STORES
    case E_PROTO_REQ_DATA_STORES
    
    /*
        Menus
    */
    case E_PROTO_REQ_HEADER_MENUS
    case E_PROTO_REQ_DATA_MENUS
    
    /*
        Comments
    */
    case E_PROTO_REQ_HEADER_COMMENTS
    case E_PROTO_REQ_DATA_COMMENTS
}

enum E_PROTO_RES_TYPE : Int
{
    case E_PROTO_RES_SUCCESS = 0
    case E_PROTO_RES_FAIL
    /*
        Sign In
    */
    case E_PROTO_RES_DATA_NOT_EXIST
    case E_PROTO_RES_DATA_NOT_FOUND_EMAIL
    case E_PROTO_RES_DATA_NOT_FOUND_NICKNAME
    case E_PROTO_RES_DATA_NOT_FOUND_CALLID
    
    /*
        Sign Up
    */
    case E_PROTO_RES_DATA_ACCOUNT_EXIST
    
    /*
        Stores
    */
    
    /*
        Menus
    */
    
    /*
        Comments
    */
}

enum E_NETWORK_RET_TYPE : Int
{
    case E_NETWORK_RET_SUCCESS = 0
    case E_NETWORK_RET_FAIL_SEND_HEADER
    case E_NETWORK_RET_FAIL_RECV_HEADER
    case E_NETWORK_RET_FAIL_RECV_DATA
    
}

class NetworkingCommunication
{
    let cNetworking = CNetworking()
    
    init(){}
    
    func networkingWithServer(strMsgData : String, nMsgType : E_PROTO_REQ_TYPE, inout strRecvMsg : String) -> Int
    {
        let defaultReceivedSize = 64
        /*
            connet to Server to get server socket
        */
        cNetworking.connectServer()
        
        /*
            send header data to server
        */
        var stReqData : BuildJSON = BuildJSON()
        let sizeToSendRealData = strMsgData.characters.count
        stReqData["request"] = String(nMsgType.rawValue)
        stReqData["size"] = String(sizeToSendRealData)
        
        let strReqJSONHeader = stReqData.toJSONString()
        let sizeToSendHeader = strReqJSONHeader.characters.count
        var nSent = cNetworking.sendData(strReqJSONHeader)
        if (nSent != sizeToSendHeader) {
            return E_NETWORK_RET_TYPE.E_NETWORK_RET_FAIL_SEND_HEADER.rawValue
        }
        
        /*
            receive response of header data from server
        */
        let strReceivedData = cNetworking.recvData(defaultReceivedSize)
        if (strReceivedData.characters.count < 1) {
            return E_NETWORK_RET_TYPE.E_NETWORK_RET_FAIL_RECV_HEADER.rawValue
        }
        
        /*
            check response from server for next step.
            To check that, we use swityjson library
        */
        let jsonMethod = JSONMethods()
        let dicData = jsonMethod.convertStringToDictionary(strReceivedData)
        let resHeaderJson = JSON(dicData!)
        let nResponseRet = (resHeaderJson.dictionary!["response"]! as JSON).int!
        if (nResponseRet != E_PROTO_RES_TYPE.E_PROTO_RES_SUCCESS.rawValue) {
            return E_NETWORK_RET_TYPE.E_NETWORK_RET_FAIL_RECV_HEADER.rawValue
        }
        
        /*
            send real data to server
        */
        nSent = cNetworking.sendData(strMsgData)
        if (nSent != sizeToSendRealData) {
            return E_NETWORK_RET_TYPE.E_NETWORK_RET_FAIL_SEND_HEADER.rawValue
        }
        
        /*
            receive response data from server
        */
        let strResponseData = cNetworking.recvData(defaultReceivedSize)
        if (strResponseData.characters.count < 1) {
            return E_NETWORK_RET_TYPE.E_NETWORK_RET_FAIL_RECV_DATA.rawValue
        }
        let dicResponseData = jsonMethod.convertStringToDictionary(strResponseData)
        let resDataJSON = JSON(dicResponseData!)
        let nResponseDataRet = (resDataJSON.dictionary!["response"]! as JSON).int!
        let nSizeOfResponseData = (resDataJSON.dictionary!["size"]! as JSON).int!
        if (nResponseDataRet != E_PROTO_RES_TYPE.E_PROTO_RES_SUCCESS.rawValue) {
            return E_NETWORK_RET_TYPE.E_NETWORK_RET_FAIL_RECV_HEADER.rawValue
        }
        
        /*
            receive response real data from server
        */
        if (nSizeOfResponseData > 0) {
            let strResponseRealData = cNetworking.recvData(nSizeOfResponseData)
            if (strResponseRealData.characters.count < 1) {
                return E_NETWORK_RET_TYPE.E_NETWORK_RET_FAIL_RECV_DATA.rawValue
            }
            strRecvMsg = strResponseRealData
        }
        return E_NETWORK_RET_TYPE.E_NETWORK_RET_SUCCESS.rawValue
    }
}






