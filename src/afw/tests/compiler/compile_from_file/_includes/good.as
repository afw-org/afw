#!/usr/bin/env afw

function func1()
{
    return true;
}

function func2()
{
    return false;
}

return {
    func1: func1,
    func2: func2
};