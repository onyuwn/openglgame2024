#ifndef DEBUGDRAWER_HPP
#define DEBUGDRAWER_HPP

#include "btBulletDynamicsCommon.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.hpp"

class DebugDrawer : public btIDebugDraw
{
public:
    DebugDrawer() : lineIndex(0), debugShader(std::make_shared<Shader>("src/shaders/physdebug.vs", "src/shaders/physdebug.fs"))
    {
        m_debugMode = DBG_DrawWireframe;
		init();
    }

    ~DebugDrawer() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

	void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix){
		this->debugShader->use();
		this->debugShader->setMat4("view", pViewMatrix);
		this->debugShader->setMat4("projection", pProjectionMatrix);
		glm::mat4 model = glm::mat4(1.0f);
		this->debugShader->setMat4("model", model);
	}

	void init() {
		// Initialize OpenGL resources (VAO, VBO, and shader program)
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		// Allocate memory for the VBO (start with an empty buffer)
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(GLfloat) * 6, nullptr, GL_STATIC_DRAW);

		// Vertex position (location 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

    void setDebugMode(int debugMode) override {
        m_debugMode = debugMode;
    }

    int getDebugMode() const override {
        return m_debugMode;
    }

    // Implement the draw method to render collision shapes
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override
    {
		GLfloat points[6] = {
			from.x(), from.y(), from.z(),  // First point (from)
			to.x(), to.y(), to.z()          // Second point (to)
		};

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);  // Vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glDrawArrays(GL_LINES, 0, 2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual void drawContactPoint(const btVector3& pointOnB,
                                  const btVector3& normalOnB,
                                  btScalar distance,
                                  int lifeTime,
                                  const btVector3& color)
    {
        // Optionally, implement contact point rendering
    }

    virtual void reportErrorWarning(const char* warningString)
    {
        printf("Bullet Warning: %s\n", warningString);
    }

    virtual void draw3dText(const btVector3& location, const char* textString)
    {
        // Optional: Implement 3D text rendering if needed
    }

private:
    int m_debugMode, lineIndex;
	GLuint vao, vbo;
	std::shared_ptr<Shader> debugShader;
};

#endif