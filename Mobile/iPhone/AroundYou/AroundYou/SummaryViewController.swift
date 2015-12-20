//
//  SummaryViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 12. 18..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class SummaryViewController: UIViewController, UIPopoverPresentationControllerDelegate {

    @IBOutlet weak var labelEvaluation: UILabel!
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        setGestureToImages()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func setGestureToImages() {
        let gestureClickedlabelEvaluation = UITapGestureRecognizer(target: self, action: "clickedLabelEvaluation:")
        labelEvaluation.addGestureRecognizer(gestureClickedlabelEvaluation)
        labelEvaluation.userInteractionEnabled = true
    }

    func clickedLabelEvaluation(sender : AnyObject) {
        debugPrint("Clicked label evaluation")
        
        let popoverContent = (self.storyboard?.instantiateViewControllerWithIdentifier("PopUpEvaluation"))! as UIViewController
        
        popoverContent.modalPresentationStyle = .Popover
        if let popover = popoverContent.popoverPresentationController {
            
            //let viewForSource = sender as! UIView
            //popover.sourceView = viewForSource
            popover.sourceView = (self.parentViewController?.view)! as UIView
    
            // the position of the popover where it's showed
            popover.sourceRect = CGRect(x: 200,y: 50,width:0,height: 0)
            popover.permittedArrowDirections = UIPopoverArrowDirection()
            
            // the size you want to display
            popoverContent.preferredContentSize = CGSizeMake(250 ,200)
            
            popover.delegate = self
        }
        self.presentViewController(popoverContent, animated: false, completion: nil)
    }
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    func adaptivePresentationStyleForPresentationController(controller: UIPresentationController) -> UIModalPresentationStyle {
            return .None
    }
}
