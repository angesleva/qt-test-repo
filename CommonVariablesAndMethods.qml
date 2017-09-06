
pragma Singleton

import QtQuick 2.2


Item {

    readonly property int label_editor: 1
    readonly property int label_label: 2
    property int screenWidth:0
    property int screenHeight:0

    //This function clears the dynamic labels and editors from the end of the form grid
    //and replaces them with new ones.
    //The texts for the new components will be read from sourceTextList.
    function replaceDynGridFields(targetGrid, typeHint, titleArray, contentArray, sourceTextList)
    {

        var length = sourceTextList.length;

        //1. Clear the dynamically created form components
        for (var i = targetGrid.children.length, j=0; j < titleArray.length*2 ; i--, j++) {
            console.log("destroying: " + i)
            targetGrid.children[i-1].destroy()
        }

        //2. Clear the arrays holding the info of the nbr of dynamic widgets.
        titleArray.length = 0;
        contentArray.length = 0;

        //3. Add the contract-dependent labels and editors at the end of the form.
        for(var k=0, l=0;k<length;k+=2,l++) {
            titleArray.push(Qt.createComponent("DynLabel.qml").createObject(targetGrid));
            titleArray[l].text = qsTr(sourceTextList[k]); //Loalizing the labels on the UI side

            if(typeHint===label_editor)
                contentArray.push(Qt.createComponent("Editor.qml").createObject(targetGrid));
            else //Type label_label
                contentArray.push(Qt.createComponent("DynLabel.qml").createObject(targetGrid));

            contentArray[l].text = qsTr(sourceTextList[k+1]); //lizing the labels on the UI side
        }
//        for(var i = 0;i<contentArray.length;i++)
//        {
//            console.log("Koordinaatti: ",i," ",contentArray[i].y);
//        }
    }

    //A function for displaying a message box
    function displayMsgBox(titleText, contentText, boxParent, connection)
    {
        var component = Qt.createComponent("MessageBox.qml");
        if (component.status === Component.Ready) {
            var dialog = component.createObject(boxParent);
            dialog.title = titleText;
            dialog.setText(contentText);
            connection.target = dialog;
            //dialog.open();
        }
    }
}


