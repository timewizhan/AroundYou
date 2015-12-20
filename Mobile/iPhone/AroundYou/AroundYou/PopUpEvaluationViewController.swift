//
//  PopUpEvaluationViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 12. 18..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class PopUpEvaluationViewController: UIViewController {

    @IBOutlet weak var imageTaste1: UIImageView!
    @IBOutlet weak var imageTaste2: UIImageView!
    @IBOutlet weak var imageTaste3: UIImageView!
    @IBOutlet weak var imageTaste4: UIImageView!
    @IBOutlet weak var imageTaste5: UIImageView!
    @IBOutlet weak var imageKind1: UIImageView!
    @IBOutlet weak var imageKind2: UIImageView!
    @IBOutlet weak var imageKind3: UIImageView!
    @IBOutlet weak var imageKind4: UIImageView!
    @IBOutlet weak var imageKind5: UIImageView!
    @IBOutlet weak var imageMood1: UIImageView!
    @IBOutlet weak var imageMood2: UIImageView!
    @IBOutlet weak var imageMood3: UIImageView!
    @IBOutlet weak var imageMood4: UIImageView!
    @IBOutlet weak var imageMood5: UIImageView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        setGestureToImagesEvalution()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func setGestureToImagesEvalution() {
        // Set taste image
        let gestureClickedTaste1 = UITapGestureRecognizer(target: self, action: "clickedTaste1:")
        let gestureClickedTaste2 = UITapGestureRecognizer(target: self, action: "clickedTaste2:")
        let gestureClickedTaste3 = UITapGestureRecognizer(target: self, action: "clickedTaste3:")
        let gestureClickedTaste4 = UITapGestureRecognizer(target: self, action: "clickedTaste4:")
        let gestureClickedTaste5 = UITapGestureRecognizer(target: self, action: "clickedTaste5:")
        
        imageTaste1.addGestureRecognizer(gestureClickedTaste1)
        imageTaste2.addGestureRecognizer(gestureClickedTaste2)
        imageTaste3.addGestureRecognizer(gestureClickedTaste3)
        imageTaste4.addGestureRecognizer(gestureClickedTaste4)
        imageTaste5.addGestureRecognizer(gestureClickedTaste5)
        
        imageTaste1.userInteractionEnabled = true
        imageTaste2.userInteractionEnabled = true
        imageTaste3.userInteractionEnabled = true
        imageTaste4.userInteractionEnabled = true
        imageTaste5.userInteractionEnabled = true
        
        // Set kind image 
        let gestureClickedKind1 = UITapGestureRecognizer(target: self, action: "clickedKind1:")
        let gestureClickedKind2 = UITapGestureRecognizer(target: self, action: "clickedKind2:")
        let gestureClickedKind3 = UITapGestureRecognizer(target: self, action: "clickedKind3:")
        let gestureClickedKind4 = UITapGestureRecognizer(target: self, action: "clickedKind4:")
        let gestureClickedKind5 = UITapGestureRecognizer(target: self, action: "clickedKind5:")
        
        imageKind1.addGestureRecognizer(gestureClickedKind1)
        imageKind2.addGestureRecognizer(gestureClickedKind2)
        imageKind3.addGestureRecognizer(gestureClickedKind3)
        imageKind4.addGestureRecognizer(gestureClickedKind4)
        imageKind5.addGestureRecognizer(gestureClickedKind5)
        
        imageKind1.userInteractionEnabled = true
        imageKind2.userInteractionEnabled = true
        imageKind3.userInteractionEnabled = true
        imageKind4.userInteractionEnabled = true
        imageKind5.userInteractionEnabled = true
        
        // Set mood image
        let gestureClickedMood1 = UITapGestureRecognizer(target: self, action: "clickedMood1:")
        let gestureClickedMood2 = UITapGestureRecognizer(target: self, action: "clickedMood2:")
        let gestureClickedMood3 = UITapGestureRecognizer(target: self, action: "clickedMood3:")
        let gestureClickedMood4 = UITapGestureRecognizer(target: self, action: "clickedMood4:")
        let gestureClickedMood5 = UITapGestureRecognizer(target: self, action: "clickedMood5:")
        
        imageMood1.addGestureRecognizer(gestureClickedMood1)
        imageMood2.addGestureRecognizer(gestureClickedMood2)
        imageMood3.addGestureRecognizer(gestureClickedMood3)
        imageMood4.addGestureRecognizer(gestureClickedMood4)
        imageMood5.addGestureRecognizer(gestureClickedMood5)
        
        imageMood1.userInteractionEnabled = true
        imageMood2.userInteractionEnabled = true
        imageMood3.userInteractionEnabled = true
        imageMood4.userInteractionEnabled = true
        imageMood5.userInteractionEnabled = true
    }
    
    func clickedTaste1(sender : AnyObject) {
        changeTasteImageAccordingToNumber(1)
    }
    func clickedTaste2(sender : AnyObject) {
        changeTasteImageAccordingToNumber(2)
    }
    func clickedTaste3(sender : AnyObject) {
        changeTasteImageAccordingToNumber(3)
    }
    func clickedTaste4(sender : AnyObject) {
        changeTasteImageAccordingToNumber(4)
    }
    func clickedTaste5(sender : AnyObject) {
        changeTasteImageAccordingToNumber(5)
    }
    
    func clickedKind1(sender : AnyObject) {
        changeKindImageAccordingToNumber(1)
    }
    func clickedKind2(sender : AnyObject) {
        changeKindImageAccordingToNumber(2)
    }
    func clickedKind3(sender : AnyObject) {
        changeKindImageAccordingToNumber(3)
    }
    func clickedKind4(sender : AnyObject) {
        changeKindImageAccordingToNumber(4)
    }
    func clickedKind5(sender : AnyObject) {
        changeKindImageAccordingToNumber(5)
    }
    
    func clickedMood1(sender : AnyObject) {
        changeMoodImageAccordingToNumber(1)
    }
    func clickedMood2(sender : AnyObject) {
        changeMoodImageAccordingToNumber(2)
    }
    func clickedMood3(sender : AnyObject) {
        changeMoodImageAccordingToNumber(3)
    }
    func clickedMood4(sender : AnyObject) {
        changeMoodImageAccordingToNumber(4)
    }
    func clickedMood5(sender : AnyObject) {
        changeMoodImageAccordingToNumber(5)
    }
    /*
        page1_023 is disable
        page1_022 is able
    */
    func changeTasteImageAccordingToNumber(selectedNumber : Int) {
        if selectedNumber == 1 {
            imageTaste1.image = UIImage(named: "page1_022")
            imageTaste2.image = UIImage(named: "page1_023")
            imageTaste3.image = UIImage(named: "page1_023")
            imageTaste4.image = UIImage(named: "page1_023")
            imageTaste5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 2 {
            imageTaste1.image = UIImage(named: "page1_022")
            imageTaste2.image = UIImage(named: "page1_022")
            imageTaste3.image = UIImage(named: "page1_023")
            imageTaste4.image = UIImage(named: "page1_023")
            imageTaste5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 3 {
            imageTaste1.image = UIImage(named: "page1_022")
            imageTaste2.image = UIImage(named: "page1_022")
            imageTaste3.image = UIImage(named: "page1_022")
            imageTaste4.image = UIImage(named: "page1_023")
            imageTaste5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 4 {
            imageTaste1.image = UIImage(named: "page1_022")
            imageTaste2.image = UIImage(named: "page1_022")
            imageTaste3.image = UIImage(named: "page1_022")
            imageTaste4.image = UIImage(named: "page1_022")
            imageTaste5.image = UIImage(named: "page1_023")
        }
        else {
            imageTaste1.image = UIImage(named: "page1_022")
            imageTaste2.image = UIImage(named: "page1_022")
            imageTaste3.image = UIImage(named: "page1_022")
            imageTaste4.image = UIImage(named: "page1_022")
            imageTaste5.image = UIImage(named: "page1_022")
        }
    }
    
    func changeKindImageAccordingToNumber(selectedNumber : Int) {
        if selectedNumber == 1 {
            imageKind1.image = UIImage(named: "page1_022")
            imageKind2.image = UIImage(named: "page1_023")
            imageKind3.image = UIImage(named: "page1_023")
            imageKind4.image = UIImage(named: "page1_023")
            imageKind5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 2 {
            imageKind1.image = UIImage(named: "page1_022")
            imageKind2.image = UIImage(named: "page1_022")
            imageKind3.image = UIImage(named: "page1_023")
            imageKind4.image = UIImage(named: "page1_023")
            imageKind5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 3 {
            imageKind1.image = UIImage(named: "page1_022")
            imageKind2.image = UIImage(named: "page1_022")
            imageKind3.image = UIImage(named: "page1_022")
            imageKind4.image = UIImage(named: "page1_023")
            imageKind5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 4 {
            imageKind1.image = UIImage(named: "page1_022")
            imageKind2.image = UIImage(named: "page1_022")
            imageKind3.image = UIImage(named: "page1_022")
            imageKind4.image = UIImage(named: "page1_022")
            imageKind5.image = UIImage(named: "page1_023")
        }
        else {
            imageKind1.image = UIImage(named: "page1_022")
            imageKind2.image = UIImage(named: "page1_022")
            imageKind3.image = UIImage(named: "page1_022")
            imageKind4.image = UIImage(named: "page1_022")
            imageKind5.image = UIImage(named: "page1_022")
        }
        
    }
    
    func changeMoodImageAccordingToNumber(selectedNumber : Int) {
        if selectedNumber == 1 {
            imageMood1.image = UIImage(named: "page1_022")
            imageMood2.image = UIImage(named: "page1_023")
            imageMood3.image = UIImage(named: "page1_023")
            imageMood4.image = UIImage(named: "page1_023")
            imageMood5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 2 {
            imageMood1.image = UIImage(named: "page1_022")
            imageMood2.image = UIImage(named: "page1_022")
            imageMood3.image = UIImage(named: "page1_023")
            imageMood4.image = UIImage(named: "page1_023")
            imageMood5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 3 {
            imageMood1.image = UIImage(named: "page1_022")
            imageMood2.image = UIImage(named: "page1_022")
            imageMood3.image = UIImage(named: "page1_022")
            imageMood4.image = UIImage(named: "page1_023")
            imageMood5.image = UIImage(named: "page1_023")
        }
        else if selectedNumber == 4 {
            imageMood1.image = UIImage(named: "page1_022")
            imageMood2.image = UIImage(named: "page1_022")
            imageMood3.image = UIImage(named: "page1_022")
            imageMood4.image = UIImage(named: "page1_022")
            imageMood5.image = UIImage(named: "page1_023")
        }
        else {
            imageMood1.image = UIImage(named: "page1_022")
            imageMood2.image = UIImage(named: "page1_022")
            imageMood3.image = UIImage(named: "page1_022")
            imageMood4.image = UIImage(named: "page1_022")
            imageMood5.image = UIImage(named: "page1_022")
        }
    }
    
    
    
    
    @IBAction func savedEvaluation(sender: AnyObject) {
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
