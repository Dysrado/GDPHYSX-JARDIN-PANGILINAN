#include "Model3D.h"

// Sets the position, scale, and rotation to te Model3D class
void Model3D::initVariables(glm::vec3 pos, glm::vec3 size, glm::vec3 rot) {
	glm::mat4 identity = glm::mat4(1.f);
	// Sets the Model3D's location, scale and rotation to the parameters location, scale and rotation
	this->position = pos;
	this->rotation = rot;
	this->scale = size;
	transform = glm::translate(identity, this->position);
	transform = glm::scale(transform, this->scale);
	transform = glm::rotate(transform, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
}

//Initializes the buffers
void Model3D::init() {
	// initialitation for the 3d Model
	std::string path = "3D/creeper.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	tinyobj::attrib_t attributes;
	bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	// initialization of VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(attributes.vertices) * attributes.vertices.size(), attributes.vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void Model3D::integrate(float duration, int type)
{

	if (duration > 0.0) {
		//std::cout << "Duration:" << duration << std::endl;
		
		//print();

		switch (type) {
			case 1: //Pistol Bullet

				positionUpdate(duration); //Updates the position of the projectile based on the time and velocity.
				acceleration = inverseMass * glm::vec3(0, -gravity.y, force); 
				velocity = acceleration * duration * duration * 0.5f;
				velocity.z *= damping;

				break;
			case 2: //Artillery Bullet
				positionUpdate(duration); //Updates the position of the projectile based on the time and velocity.
				acceleration = inverseMass * glm::vec3(0, force, force);
				acceleration.y = inverseMass * -gravity.y;
				velocity = acceleration * duration * duration * 0.5f;
				velocity.z *= damping;
				break;
			case 3: //Fireball
				positionUpdate(duration); //Updates the position of the projectile based on the time and velocity.
				acceleration = inverseMass * glm::vec3(0, force, force);
				velocity = acceleration * duration * duration * 0.5f;
				velocity.y *= damping;
				velocity.z *= damping;
				break;
			case 4: //Laser
				positionUpdate(duration); //Updates the position of the projectile based on the time and velocity.
				acceleration = inverseMass * glm::vec3(0, 0, force);
				velocity = acceleration * duration * duration * 0.5f;
				
				break;
			

		}

		
		//velocity = acceleration * duration * duration * 0.5f;
		/*acceleration.z = inverseMass * force;
		
		acceleration = inverseMass * glm::vec3(force/2,0,force);
		acceleration.y = inverseMass * -gravity.y;*/
		


		//velocity.x *= damping;
		//velocity.y *= damping;
		//velocity.z *= damping;
		
		transform = glm::translate(transform, position);
	}
	
}

void Model3D::positionUpdate(float duration)
{
	//.this->position += velocity * duration + acceleration * duration * duration * 0.5f;
	position += velocity * duration;



}



// Renders or draws the model
void Model3D::render(GLuint shaderProgram) {
	// set the location
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// use the shader from the main code
	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
};

// Delete VAO, VBO, and EBO
void Model3D::deleteVertex() {
	// delete the buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
void Model3D::print()
{
	std::cout << "Position X: " << this->position.x << std::endl
		<< "Position Y: " << this->position.y << std::endl
		<< "Position Z: " << this->position.z << std::endl;
}
;