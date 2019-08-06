#ifndef TASKINFO_H
#define TASKINFO_H

#include <functional>

namespace Async
{

	class CancellationToken
	{
		public:
			CancellationToken()
			{
			}
			~CancellationToken()
			{
			}

			bool isCancellable()
			{
				return true;
			}
	};


	class ITaskInfo
	{
		public:
			ITaskInfo(){}

			virtual ~ITaskInfo(){}
			virtual void executeTask() = 0;
	};


	template<typename Task>
	class TaskInfo : public ITaskInfo
	{
			Task mTask;
		public:
			TaskInfo(Task task):mTask(task)
			{
			}	

			~TaskInfo()
			{
			}

			void executeTask() override
			{
				this->mTask();
			}
	};


	template<typename Task,typename Response>
	class TaskInfoResponse : public ITaskInfo
	{
		Task mTask;
		Response mResponse;

	public:

		TaskInfoResponse(Task task,Response response): mTask(std::forward<Task>(task)),mResponse(std::forward<Response>(response))
		{

		}

		~TaskInfoResponse()
		{
		}

		void executeTask() override
		{
			this->mResponse(this->mTask());
		}
	};


	template<typename Task>
        class TaskInfoAck : public ITaskInfo
        {
                Task mTask;
                std::function<void()> mResponse;
        public:
                TaskInfoAck(Task task,std::function<void()> response): mTask(std::forward<Task>(task)),mResponse(response)
                {

                }

                ~TaskInfoAck()
                {
                }

                void executeTask() override
                {
                        this->mTask();
			this->mResponse();
                }
        };


	template<typename Fn>
	class TaskInfoCancellable : public ITaskInfo
	{
                        Fn mTask;
			std::weak_ptr<CancellationToken> mToken;
                public:
                        TaskInfoCancellable(Fn task,std::shared_ptr<CancellationToken> token):mTask(task),mToken(token)
                        {
                        }

                        ~TaskInfoCancellable()
                        {
                        }

                        void executeTask() override
                        {
				if(!this->mToken.expired())
				{
                                	this->mTask();
				}
                        }
        };
}
#endif
