// Copyright 2014 Archangel.SDY@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef DISPATCHER_H
#define DISPATCHER_H

class MockResponse;
class RecordedRequest;

class Dispatcher
{
public:
    virtual ~Dispatcher() {}

    virtual MockResponse dispatch(const RecordedRequest &request) = 0;
    virtual MockResponse peek();
};

#endif // DISPATCHER_H
