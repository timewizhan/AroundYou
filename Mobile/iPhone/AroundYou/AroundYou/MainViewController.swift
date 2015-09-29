//
//  MainViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MainViewController: UIViewController, UIPageViewControllerDelegate, UIPageViewControllerDataSource {
    
    
    @IBOutlet weak var mySubView: UIView!
    var pageController:UIPageViewController?
    var pageContent = NSArray()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        self.createContentPages()
        
        self.pageController = UIPageViewController(transitionStyle: .Scroll, navigationOrientation: .Horizontal, options: nil)
        
        self.pageController?.delegate = self
        self.pageController?.dataSource = self
        
        let startingViewController: StoreViewController = self.viewControllerAtIndex(1)!
        let viewControllers: NSArray = [startingViewController]
        
        self.pageController!.setViewControllers(viewControllers as? [UIViewController], direction: .Forward, animated: false, completion: nil)
        
        self.mySubView.addSubview(self.pageController!.view)
        
        let pageViewRect = self.mySubView.bounds
        self.pageController!.view.frame = pageViewRect
        self.pageController!.didMoveToParentViewController(self)
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    func createContentPages() {
        var pageStrings = [String]()
        for i in 1...11
        {
            let contentString = "<html><head></head><body><br><h1>Chapter \(i)</h1><p>This is the page \(i) of content displayed using UIPageViewController in </p></body></html>"
            pageStrings.append(contentString)
        }
        pageContent = pageStrings
    }
    
    func viewControllerAtIndex(index: Int) -> StoreViewController? {
        if (self.pageContent.count == 0 || index >= self.pageContent.count) {
            return nil
        }
        
        let storyBoard = UIStoryboard(name: "Main", bundle: NSBundle.mainBundle())
        let dataViewController = storyBoard.instantiateViewControllerWithIdentifier("StoreViewController") as! StoreViewController
        
        dataViewController.dataObject = pageContent[index]
        return dataViewController
    }
    
    func indexOfViewController(viewController : StoreViewController) -> Int {
        if let dataObject:AnyObject = viewController.dataObject {
            return self.pageContent.indexOfObject(dataObject)
        } else {
            return NSNotFound
        }
    }
    
    func pageViewController(pageViewController: UIPageViewController, viewControllerBeforeViewController viewController: UIViewController) -> UIViewController? {
        var index = self.indexOfViewController(viewController as! StoreViewController)
        if (index == 0) || (index == NSNotFound) {
            return nil
        }
        index--
        return self.viewControllerAtIndex(index)
    }
    
    func pageViewController(pageViewController: UIPageViewController, viewControllerAfterViewController viewController:UIViewController) -> UIViewController? {
        var index = self.indexOfViewController(viewController as! StoreViewController)
        if index == NSNotFound {
            return nil
        }
        
        index++
        if index == self.pageContent.count {
            return nil
        }
        return self.viewControllerAtIndex(index)
    }
    
}

