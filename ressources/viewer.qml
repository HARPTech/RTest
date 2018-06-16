import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    id: root

    Item {
	id: roverScale

	anchors.fill: parent

	// Main rover body as a rectangle.
	Rectangle {
	    id: roverBody
	    width: 120
	    height: 170

	    anchors.horizontalCenter: parent.horizontalCenter
	    anchors.verticalCenter: parent.verticalCenter

	    color: "darkgrey"

	    MouseArea {
		anchors.fill: parent
		onClicked: {
		    testText.text = "Test" + RoverModel
		}
	    }
	}

	GridView {
	    x: roverBody.x - 2.5
	    y: roverBody.y - 10

	    width: roverBody.width * 2
	    height: roverBody.height * 2

	    cellWidth: roverBody.width
	    cellHeight: roverBody.height

	    model: RoverModel
	    delegate: Rectangle {
		id: wheelDelegate
		width: 5
		height: 20
 		color: "black"

		transform: Rotation {
		    angle: {
			return (steering - 128) / 2
		    }
		    origin.x: width / 2
		    origin.y: height / 2
		}

		Rectangle {
		    // This rectangle is the state display.
		    anchors.horizontalCenter: parent.horizontalCenter
		    anchors.verticalCenter: parent.verticalCenter

		    width: 3
		    height: Math.abs(speed)
		    color: {
			if(speed >= 0) {
			    return "lightgreen"
			}
			else {
			    return "purple"
			}
		    }
		}
	    }
	}
    }

}
