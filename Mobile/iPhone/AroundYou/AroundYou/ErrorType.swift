//
//  ErrorType.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 2..
//  Copyright © 2015년 cclab. All rights reserved.
//

import Foundation

enum E_RET_TYPE : Int
{
    case E_RET_SUCCESS = 0
    case E_RET_FAIL
    
    case E_RET_UNKNOWN
}

enum E_DATA_CONNECTION_TYPE
{
    case E_DATA_READY
    case E_DATA_START
    case E_DATA_DOWNLOADING
    case E_DATA_FINISH
    case E_DATA_CANCELL
}

enum E_REFRESH_TYPE
{
    case E_REFRESH_READY
    case E_REFRESH_DONE
}