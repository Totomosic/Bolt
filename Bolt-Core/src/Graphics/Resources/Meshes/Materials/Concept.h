/*

	Material
		Defines
		Shader Effect
			Defines
			Shader Technique
				Defines
				vector<Passes>
					Defines
					blend
					depth
					cull
					ShaderProgram
						vector<Stream>
						vector<Uniform>

*/

/*

	ShaderFactory f;
	VertexShader& vertex = f.Vertex();
	ShaderFuncResultPtr worldPosition = ShaderOps::Mul().Result({ vertex.RendererUniform(RendererUniform::ModelMatrix), vertex.Position() });
	vertex.SetVertexPosition(worldPosition);
	ShaderPassValuePtr passedWorldPosition = vertex.Pass(worldPosition);

*/

/*

	MaterialFactory m;
	m.CurrentShader(ShaderType::Vertex);
	std::shared_ptr<ShaderOperation> matMult = m.Operation<MatrixMultiplyOperation>();
	std::shared_ptr<ShaderVar> position = m.GetStream(ShaderStream::Position);
	std::shared_ptr<ShaderVar> worldPosition = matMult.Execute(m.RendererUniform(RendererUniform::ModelMatrix), position);
	std::shared_ptr<ShaderVar> constantValue = m.UserUniform("Constant", Type::float);
	m.SetAttr(ShaderAttribute::Position, worldPosition);
	m.Pass(ShaderType::Fragment, worldPosition);

	m.CurrentShader(ShaderType::Fragment);
	std::shared_ptr<ShaderVar> worldPos = m.GetPass(Shader::Vertex, 0);
	std::shared_ptr<ShaderVar> baseColor = m.RendererUniform(RendererUniform::Color);
	m.SetAttr(ShaderAttribute::FragColor, baseColor);
	m.Finalize(); // Ensures completeness

	Material instance = m.Create();
	instance.Link("Constant", &instance.Values()["FPS"]);

*/