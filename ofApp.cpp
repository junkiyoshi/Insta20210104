#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(1.5);

	this->near_line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	this->line_list.clear();
	this->near_line.clear();

	auto noise_seed_deg = ofRandom(1000);
	auto noise_seed_radius = ofRandom(1000);
	vector<glm::vec3> vertices;
	for (int i = 0; i < 50; i++) {

		auto noise_deg = ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i) * 0.005), 0, 1, -360, 360);
		auto noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i) * 0.008), 0, 1, -350, 350);
		vertices.push_back(glm::vec3(noise_radius * cos(noise_deg * DEG_TO_RAD), noise_radius * sin(noise_deg * DEG_TO_RAD), 0));
	}

	for (int deg = 0; deg < 360; deg += 45) {

		auto rotation = glm::rotate(glm::mat4(), deg * (float)DEG_TO_RAD , glm::vec3(0, 0, 1));
		ofMesh line;
		line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		
		for (int i = 0; i < vertices.size(); i++) {

			line.addVertex(glm::vec4(vertices[i], 0) * rotation + glm::vec4(ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0, 0));
			if (i > 0) {

				line.addIndex(i); line.addIndex(i - 1);
			}
		}

		this->line_list.push_back(line);
	}

	for (int i = 0; i < this->line_list.size(); i++) {

		for (int k = i + 1; k < this->line_list.size(); k++) {

			for (int m = 0; m < this->line_list[i].getNumVertices(); m++) {

				for (int p = 0; p < this->line_list[k].getNumVertices(); p++) {

					if (glm::distance(this->line_list[i].getVertex(m), this->line_list[k].getVertex(p)) < 30) {

						this->near_line.addVertex(this->line_list[i].getVertex(m));
						this->near_line.addVertex(this->line_list[k].getVertex(p));

						this->near_line.addIndex(this->near_line.getNumVertices() - 1);
						this->near_line.addIndex(this->near_line.getNumVertices() - 2);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (auto& line : this->line_list) {

		line.draw();
	}

	this->near_line.draw();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}