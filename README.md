<h1>Overview</h1>
A very simple unreal project designed to send simple xapi statements to an lrs.

<h1>XAPIManager Actor</h1>

The XAPIManager actor can be found in the place actors tab, add this to the level.
<br>
<br>
<img width="508" alt="image1" src="https://user-images.githubusercontent.com/39784801/136555156-cd2dcceb-66b4-40f4-a42c-203635f8d033.png">
<br>
<br>
Make sure to set the URL, BASIC AUTH, and the XAPI VERSION NUMBER in the inspector or statements will not work.
<br>

<img width="365" alt="image2" src="https://user-images.githubusercontent.com/39784801/136555175-5725d898-bb39-4b46-ba8a-6202b1e396fa.png">
<br>
 <h1>CreateXAPIPhrase</h1>

The main function to send a statement to the lrs is contained within the XAPIManager and called CreateXAPIPhrase, so if you’d like to customize it to fit your needs just modify the code within this function. The CreateXAPIPhrase is both callable from C++ and blueprints explained below how to do that.

I’m going to break down the parameters of this function below.
<br>

FString Activity - The activity is what the user is doing. Example :“Solo Hang Gliding”. Default is “NOACTIVITY” if not set.
<br>

FString AgentName - The name of the user from the statement. Example:  “Jon Doe”. Default is “NOAGENT” if not set.
<br>

FString Email - The email of the user. Example : “Fakeemail@notarealemail.com” . Default is “NoEmail@email.com” if not set.
<br>

FString VerbName - The action the user is doing. Example : “completed”. Default is “NOVERB” if not set.
<br>

FString VerbURL - The URL of the verb. Example And Default : http://adlnet.gov/expapi/verbs/“” 
<br>

FString CourseURL - The URL of the course. Example and Default : “http://adlnet.gov/expapi/activities/course” 
<br>

Float TimeToComplete - The time it took for the user to do an activity. Default : This is how long the level was running for.
<br>

FString CurrentDateTime - The date the activity was completed. Default : Is the current datetime, just thought I’d let users set this incase they want to override the current date.
<br>


<h1>Example Levels</h1>

Both the example levels included in the project are just blank levels with a button that you click that sends a statement to an endpoint. One level uses blueprints and the other uses c++.

<h2>XAPIUI_ExampleLevelC++</h2>

The c++ level handles everything via code other than instantiating the UI which is done in the level blueprint by adding the XAPIUIController. To find the code for the button and how it sends requests that way, open up the c++ class XAPIUIController. This class sets up an event for the button that calls the CreateXAPIPhrase function when clicked it also instantiates the ui widget itself.


<h2>XAPIUsingBlueprintNodes</h2>

This level uses no c++ and just calls the CreateXAPIPhrase via blueprint. To view how this is done view the level blueprint, there it creates the ui element that handles the event. 
<br>
<br>
![creatingui](https://user-images.githubusercontent.com/39784801/146242433-45b5fb86-bda5-468e-bdd3-5fa12fe10848.png)
<br>
<br>



To view how the ui element does it go to UI/WBP_XAPINoCode and open that widget, open the graph to see how it calls the xapi phrase from an onclick. 

<br>
<br>

![creatingphrase](https://user-images.githubusercontent.com/39784801/146242455-461fcb11-6500-46e9-a8f3-b7fd395a6e85.png)

<br>




<h1> How to call statement via C++</h1>
Add the XAPIManger to a level (Can be found if you search for it in the PlaceActors area)
<br>
<br>
<img width="508" alt="image1" src="https://user-images.githubusercontent.com/39784801/136555156-cd2dcceb-66b4-40f4-a42c-203635f8d033.png">
<br>




Now set up the endpoint and the basic auth key which can be found in your lrs settings.
The end point HAS TO BE THE url/statments so for example it would be https://insertmylrshere.net/data/xAPI/statements, in the unity package the url I used was https://insertmylrshere.net/data/xAPI causing some confusion here for myself.
<br>
<br>

<img width="365" alt="image2" src="https://user-images.githubusercontent.com/39784801/136555175-5725d898-bb39-4b46-ba8a-6202b1e396fa.png">
<br>

To reference in C++ just add the XAPIManger as a UProperty and set it via the inspector
<br>
<br>

<img width="258" alt="iamge3" src="https://user-images.githubusercontent.com/39784801/136555190-f52f98ad-2e6f-43b8-8273-5a18991f2501.png">

<br>

Then call the CreateXAPIPhrase function to send info to the lrs
<br>
<br>

![newxapiunrealscreenshot](https://user-images.githubusercontent.com/39784801/146002697-05cc509c-3357-4aef-95ca-ff4f85d92b23.png)

<h1>Call XAPI Statements via blueprints</h1>

Add the XAPIManger to a level (Can be found if you search for it in the PlaceActors area)
<br>
<br>
<img width="508" alt="image1" src="https://user-images.githubusercontent.com/39784801/136555156-cd2dcceb-66b4-40f4-a42c-203635f8d033.png">
<br>

Now to call the CreateXAPIStatement via blueprints, find the xapi manager by using the node GetActorOfClass

<br>

![getactorofclass](https://user-images.githubusercontent.com/39784801/146206511-6fb0b8f9-f135-48a5-a323-350a7e28780d.png)

<br>

Now with a reference to the XAPIManger you can call the CreateXAPIPhrase node which looks like this.

<br>



![createphrase](https://user-images.githubusercontent.com/39784801/146208837-582033f8-f7c3-4afb-8f34-c7f145785bf8.png)

