package logging

//go:generate sh -c "mockgen -package logging -self_package github.com/fkwhite/Quic_GO/logging -destination mock_connection_tracer_test.go github.com/fkwhite/Quic_GO/logging ConnectionTracer"
//go:generate sh -c "mockgen -package logging -self_package github.com/fkwhite/Quic_GO/logging -destination mock_tracer_test.go github.com/fkwhite/Quic_GO/logging Tracer"
