#ifndef INDUI_OPTION_FUNCTION_H
#define INDUI_OPTION_FUNCTION_H

class optionFunction : public optionSelection{
private:

	indui_function func;
	indui_callback *callback;
public:
	optionFunction(string* label,indui_function &func) :
		optionSelection(label),
		func(func),
		callback(new indui_callback(NULL,NULL)){

        callback->caller = this;
	}


	optionFunction(string* label,indui_function &func,indui_callback *callback):
		optionSelection(label),
		func(func),
		callback(callback) {
		callback->caller = this;
	}


	bool onClick(){
		indui_function func_inst = *func;

		func_inst((void*)callback);
		return true;
	}
};



#endif // INDUI_OPTION_FUNCTION_H
