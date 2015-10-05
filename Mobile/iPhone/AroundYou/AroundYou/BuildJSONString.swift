//
//  BuildJSONString.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 2..
//  Copyright © 2015년 cclab. All rights reserved.
//

import Foundation


struct BuildJSON
{
    var dicJSONData : [String : String]
    
    init () {
        dicJSONData = [String : String]()
    }
    
    subscript (strIndex : String) -> String
        {
        get {
            return dicJSONData[strIndex]!
        }
        set (newValue){
            dicJSONData[strIndex] = newValue
        }
    }
    
    func toJSONString() -> String
    {
        var JSONString : String
        JSONString = "{"
        
        let dicTotalCount : Int = dicJSONData.count
        var i : Int = 1
        for (key, value) in dicJSONData
        {
            JSONString += "\"" + key + "\"" + ":" + "\"" + value + "\""
            if (i < dicTotalCount) {
                JSONString += ","
            }
            i++
        }
        JSONString += "}"
        
        return JSONString
    }
}

