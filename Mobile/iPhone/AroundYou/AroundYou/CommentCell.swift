//
//  CommentCell.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 5..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class CommentCell: UITableViewCell {

    @IBOutlet weak var labelCommentName: UILabel!
    @IBOutlet weak var labelCommentStar: UILabel!
    @IBOutlet weak var textFieldCommentContents: UITextField!
    
    var comment: Comment! {
        didSet {
            labelCommentName.text = comment.userId
            labelCommentStar.text = comment.strStar
            textFieldCommentContents.text = comment.strComment
        }
    }
}