// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//External
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>

//_________________________Entity Class___________________________
class Entity
{
private:

	//________________Members____________
	int m_Id_counter = 0;
	int m_Num_components = 0;
	int m_Id = 0;

public:
	//________________Members____________

	//________________Constructor/destructor____________
	Entity() {};
	~Entity() {};

	//________________Getters/Setters____________
	int GetId() const { return m_Id; }
	int SetId(int i)
	{
		m_Id_counter++;
		return m_Id = i;
	};
};

